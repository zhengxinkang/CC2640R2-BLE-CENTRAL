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

static PIN_Config GpioPinTable[] =
{
    PORT_MOTOR_FORWARD      | PIN_INPUT_EN          | PIN_PULLUP    | PIN_IRQ_DIS   | PIN_HYSTERESIS,
    PORT_MOTOR_REVERSAL     | PIN_INPUT_EN          | PIN_PULLUP    | PIN_IRQ_DIS   | PIN_HYSTERESIS,
    PORT_ANTI_LOCK          | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW  | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PORT_PREVENT_DISMANTLE  | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW  | PIN_PUSHPULL  | PIN_DRVSTR_MAX,
    PIN_TERMINATE//ÖÕÖ¹·û
};

static PIN_Handle hGpioPin = NULL;
static PIN_State pinState;

uint8_t Driver_gpioInit()
{
    hGpioPin = PIN_open(&pinState, GpioPinTable);
    uint8_t ret = true;
    if (NULL == hGpioPin)
    {
        ret = false;
    }
    return ret;
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
