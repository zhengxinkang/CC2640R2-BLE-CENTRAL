/*
 * Driver_gpio.c
 *
 *  Created on: 2018年9月12日
 *      Author: zheng
 */

#include "Driver_gpio.h"
#include "Trace.h"
#include "Types.h"
#include "MyEventConfig.h"
#include "Test_process.h"
#include "TaskTest.h"

#define PORT_ADC_DR                             IOID_0
#define PORT_ADC_CS                             IOID_1
//#define PORT_ADC_SCK                            IOID_27
//#define PORT_ADC_SDO                            IOID_28
//#define PORT_ADC_SDI                            IOID_29
//#define PORT_ADC_RST                            IOID_30

static PIN_Config GpioPinTable[] =
{
    PORT_OLED_SCLK                  | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_OLED_SDIN                  | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_OLED_RST                   | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_OLED_DC                    | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_OLED_CS                    | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,

//    PORT_AUDIO_DATA                 | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
//    PORT_AUDIO_CS                   | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
//    PORT_AUDIO_CLK                  | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,

//    PORT_SPI_DATA                   | PIN_INPUT_EN          | PIN_PULLUP        | PIN_IRQ_DIS   | PIN_HYSTERESIS,
//    PORT_SPI_CLK                    | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_SP_OUT                     | PIN_INPUT_EN          | PIN_NOPULL        | PIN_IRQ_NEGEDGE| PIN_HYSTERESIS,
    PORT_HC595_SRCLRn               | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_HC595_SRCLK                | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_HC595_RCLK                 | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_HC595_SER                  | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,

    PORT_HC165_SL                   | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_HC165_CLK                  | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_HC165_QH                   | PIN_INPUT_EN          | PIN_PULLUP        | PIN_IRQ_DIS   | PIN_HYSTERESIS,

    PORT_ADC_DR                     | PIN_INPUT_EN          | PIN_NOPULL        | PIN_IRQ_DIS   | PIN_HYSTERESIS,
    PORT_ADC_CS                     | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_ADC_SCK                    | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_ADC_SDO                    | PIN_INPUT_EN          | PIN_NOPULL        | PIN_IRQ_DIS   | PIN_HYSTERESIS,
    PORT_ADC_SDI                    | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_ADC_RST                    | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,

    PORT_MOTOR_FORWARD              | PIN_INPUT_EN          | PIN_PULLUP        | PIN_IRQ_DIS   | PIN_HYSTERESIS,
    PORT_MOTOR_REVERSAL             | PIN_INPUT_EN          | PIN_PULLUP        | PIN_IRQ_DIS   | PIN_HYSTERESIS,
    PORT_ANTI_LOCK                  | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_PREVENT_DISMANTLE          | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,

    PORT_TEST_OBLIQUE_LOCK_TONGUE   | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_TEST_SETTING_BUTTON        | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_TEST_CLEAR_BUTTON          | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,

    PORT_TEST_RESET_TARGET          | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_HIGH     | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_TEST_START                 | PIN_INPUT_EN          | PIN_PULLUP        | PIN_IRQ_NEGEDGE| PIN_HYSTERESIS,
    PORT_TEST_MODEL_S31             | PIN_INPUT_EN          | PIN_PULLUP        | PIN_IRQ_DIS   | PIN_HYSTERESIS,
    PORT_TEST_MODEL_S31X            | PIN_INPUT_EN          | PIN_PULLUP        | PIN_IRQ_DIS   | PIN_HYSTERESIS,

    PIN_TERMINATE//终止符
};

static PIN_Handle hGpioPin = NULL;
static PIN_State pinState;
static void callbackFxn(PIN_Handle handle, PIN_Id pinId);

uint8_t Driver_gpioInit()
{
    hGpioPin = PIN_open(&pinState, GpioPinTable);
    uint8_t ret = true;
    if (NULL == hGpioPin)
    {
        ret = false;
    }
    /* Setup callback for button pins */
    if (PIN_registerIntCb(hGpioPin, callbackFxn) != 0)
    {
        /* Error registering button callback function */
        ret = false;
    }
    return ret;
}

//PORT_TEST_START pin Cb >>> PORT_TEST_START
static void callbackFxn(PIN_Handle handle, PIN_Id pinId)
{
    if(PORT_SP_OUT == pinId)
    {
        TRACE_CODE("声音输入中断。\n");
        TestEvent_post(EVENT_TESTPROCESS_EVENT_VOICE);
    }
}

uint8_t Driver_gpioSet(PIN_Id pinId, PortValue portValue)
{
    uint8_t ret = true;
    if(NULL == hGpioPin)
    {
        ret = false;
    }
    else
    {
        PIN_setOutputValue(hGpioPin, pinId, portValue);
    }
    return ret;
}

PortValue Driver_gpioGet(PIN_Id pinId)
{
    PortValue portValue;
    portValue = (PortValue)PIN_getInputValue(pinId);
    return portValue;
}
