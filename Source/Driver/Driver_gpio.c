/*
 * Driver_gpio.c
 *
 *  Created on: 2018Äê9ÔÂ12ÈÕ
 *      Author: zheng
 */

#include "Driver_gpio.h"
#include "Trace.h"
#include "Types.h"
#include "MyEventConfig.h"
#include "Test_process.h"
#include "TaskTest.h"

static PIN_Config GpioPinTable[] =
{
    PORT_POWER_BAT                  | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_POWER_USB                  | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW      | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
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

    PIN_TERMINATE//ÖÕÖ¹·û
};

static PIN_Handle hGpioPin = NULL;
static PIN_State pinState;
static void TestStart_pinCbFxn(PIN_Handle handle, PIN_Id pinId);

uint8_t Driver_gpioInit()
{
    hGpioPin = PIN_open(&pinState, GpioPinTable);
    uint8_t ret = true;
    if (NULL == hGpioPin)
    {
        ret = false;
    }
    /* Setup callback for button pins */
    if (PIN_registerIntCb(hGpioPin, TestStart_pinCbFxn) != 0)
    {
        /* Error registering button callback function */
        ret = false;
    }
    return ret;
}

//PORT_TEST_START pin Cb >>> PORT_TEST_START
static void TestStart_pinCbFxn(PIN_Handle handle, PIN_Id pinId)
{
    if(!IsBusy_testProcess())
        TestEvent_post(EVENT_START_TEST);
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
