/*
 * Driver_uart.c
 *
 *  Created on: 2018年7月9日
 *      Author: zheng
 */

#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include "BF_Util.h"
#include "Driver_uart.h"
#include "Board.h"
#include "util.h"
#include "String.h"
#include "Trace.h"


static UART_Handle uartComHandle = NULL;
static UART_Params uartComParams;

static uint8_t uartRxBytesLen = 0;
static uint8_t app_rx_buf[MAX_NUM_UART_READ];
static uint8_t uart_rx_tmp[MAX_NUM_UART_READ];
static bool uartReadEnable = false;

static uint8_t readCB_Index;

static void Uart_read_callback(UART_Handle handle, void *rxBuf, size_t size);
static void Uart_read_handler(void);
static uartProtocol_CB_t uartProtocol_CB = NULL;

void Driver_uart_init(uint32_t bps, uartProtocol_CB_t uartProCallback)
{
    BF_UtilRegeistItrCallback(Uart_read_handler, &readCB_Index);
    uartProtocol_CB = uartProCallback;

    Board_initUART();
    UART_Params_init(&uartComParams);
    uartComParams.baudRate = bps;

    uartComParams.readMode = UART_MODE_CALLBACK;
    uartComParams.writeMode = UART_MODE_BLOCKING; //UART_MODE_CALLBACK

    uartComParams.readDataMode = UART_DATA_BINARY;
    uartComParams.writeDataMode = UART_DATA_BINARY;

    uartComParams.readReturnMode = UART_RETURN_FULL;
    uartComParams.readCallback  = Uart_read_callback;

    uartComParams.dataLength = UART_LEN_8;
    uartComParams.stopBits = UART_STOP_ONE;
    uartComParams.readEcho = UART_ECHO_OFF;
}

void Driver_uart_send(const uint8_t *data, uint8_t len)
{
    if (uartComHandle != NULL)
        UART_write(uartComHandle, data, len);
    else
        TRACE_DEBUG("uart not opened!\n");
}

void Driver_uart_close()
{
    if (uartComHandle != NULL)
    {
        if (uartReadEnable)
        {//最后一次调用UART_readCancel而触发的中断中，不允许再调用Uart_read，否则无法进入低功耗
            uartReadEnable = false;
        }
        UART_readCancel(uartComHandle);
        UART_close(uartComHandle);
        uartComHandle = NULL;
        Power_releaseConstraint(PowerCC26XX_SB_DISALLOW);
    }
}

void Driver_uart_open()
{
    if (uartComHandle == NULL)
    {
        Power_setConstraint(PowerCC26XX_SB_DISALLOW);
        uartComHandle = UART_open(Board_UART0, &uartComParams);
        UART_control(uartComHandle, UARTCC26XX_CMD_RETURN_PARTIAL_ENABLE, NULL);
        UART_read(uartComHandle, uart_rx_tmp, MAX_NUM_UART_READ);
        uartReadEnable = true;
    }
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void Uart_read_callback(UART_Handle handle, void *rxBuf, size_t size)
{
    if(0 != size)
    {
        uartRxBytesLen = size;
        memcpy(app_rx_buf, rxBuf, uartRxBytesLen);
        BF_UtilWaitHandle(readCB_Index);
    }

    if(uartReadEnable)//关闭串口调用read canel而触发的回调不能再调用UART_read，否则无法进入低功耗
    {
        UART_read(uartComHandle, uart_rx_tmp, MAX_NUM_UART_READ);
        //UART_read只是为下一次中断读取做准备
    }
}

static void Uart_read_handler(void)
{
//    TRACE_DEBUG("Uart0 recieve %d data:",uartRxBytesLen);
//    TRACE_DEBUG_HEXGROUP(app_rx_buf, uartRxBytesLen, ' ');
    if ((uartProtocol_CB != NULL) && (uartRxBytesLen > 0))
    {
        (*uartProtocol_CB)(app_rx_buf, uartRxBytesLen);
        uartRxBytesLen = 0;
    }
}
