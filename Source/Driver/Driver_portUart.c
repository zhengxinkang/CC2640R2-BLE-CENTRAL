/*
 * Driver_portUart.c
 *
 *  Created on: 2018Äê8ÔÂ16ÈÕ
 *      Author: zheng
 */

#include <Driver_portUart.h>
#include <ti/drivers/PIN.h>
#include "Board.h"
#include "Trace.h"
#include "Types.h"
#include "ConfigProtocol.h"
#if (BLE_UART_MODE == BLE_UART_MODE_SLEEP)
PIN_Config uartPinCfg[] =
{
    BOARD_PORT_UART_RX | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW   | PIN_PUSHPULL      | PIN_DRVSTR_MAX,
    BOARD_PORT_UART_TX | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW   | PIN_PUSHPULL      | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

static PIN_State  uartPinState;
static PIN_Handle uartPinHandle = NULL;

uint8_t Driver_uartGpio_open()
{
    uint8_t ret = true;
    if(NULL == uartPinHandle)
    {
        uartPinHandle = PIN_open(&uartPinState, uartPinCfg);

        if (NULL == uartPinHandle)
        {
            ret = false;
            TRACE_DEBUG("UartGpio_open ERROR!\n");
        }
        else
        {
//            TRACE_DEBUG("UartGpio_open success.\n");
        }
    }
    else
    {
//        TRACE_DEBUG("UartGpio_open ERROR! gpio is opened.\n");
    }

    return ret;
}

uint8_t Driver_uartGpio_close()
{
    uint8_t ret = true;
    if(NULL != uartPinHandle)
    {
        PIN_close(uartPinHandle);
        uartPinHandle = NULL;
        if (NULL != uartPinHandle)
        {
            ret = false;
            TRACE_DEBUG("UartGpio_close ERROR!\n");
        }
        else
        {
//            TRACE_DEBUG("UartGpio_close success.\n");
        }
    }
    else
    {
//        TRACE_DEBUG("UartGpio_close ERROR! gpio is closed.\n");
    }
    uartPinHandle = NULL;
    return ret;
}

uint8_t Driver_uartGpioSetLow()
{
    uint8_t ret = true;
    if(NULL == uartPinHandle)
    {
        ret = false;
    }
    else
    {
        PIN_setOutputValue(uartPinHandle, BOARD_PORT_UART_RX, 0);
        PIN_setOutputValue(uartPinHandle, BOARD_PORT_UART_TX, 0);
    }
    return ret;
}

uint8_t Driver_uartGpioSetHigh()
{
    uint8_t ret = true;
    if(NULL == uartPinHandle)
    {
        ret = false;
    }
    else
    {
        PIN_setOutputValue(uartPinHandle, BOARD_PORT_UART_RX, 1);
        PIN_setOutputValue(uartPinHandle, BOARD_PORT_UART_TX, 1);
    }
    return ret;
}


#endif//BLE_UART_MODE
