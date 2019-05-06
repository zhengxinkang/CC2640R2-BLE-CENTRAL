/*
 * Task_UartEmulator.c
 *
 *  Created on: 2018.7.5
 *      Author: blue.zhengxinkang
 */
/***include*****************************************************/
#include "TaskConfig.h"
#include "Types.h"
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Queue.h>
#include "Driver_UartEmulator.h"
#include "Board.h"
#include "MyEventConfig.h"

/***define*****************************************************/
#define BV(n)               (1 << (n))

/***static*****************************************************/
//task
Task_Struct consoleTask;
Char consoleTaskStack[CONSOLE_TASK_STACK_SIZE]={1};

// Semaphore used to wait for Sensor Controller task ALERT event
static Semaphore_Struct semScTaskAlert;

//uartEmulator
static char uartEnulator_rxBuf[UART_EMULATOR_RX_LEN];
static char uartEnulator_txBuf[UART_EMULATOR_TX_LEN]={1};
Uart_Rx_State uart_rxState = UART_RX_STATE_EMPTY;
static uint32_t uartEnulator_txBufIndex = 0;
static OnReceived uartEmulator_recieveToUp = NULL;
static uint8_t isUartEmulatorInited = 0;
/***declaration**********************************************/
void Task_uartEmulator_txHandle();
void Task_uartEmulator_rxHandle();

/***callBack************************************************/
void scCtrlReadyCallback(void) {

} // scCtrlReadyCallback

void scTaskAlertCallback(void) {
    uart_rxState = UART_RX_STATE_WAIT_FOR_GET;
    // Wake up the OS task
    MyEvent_post( MY_UARTEMULATRO_EVT );
} // scTaskAlertCallback

/***Func************************************************/
extern void MyEvent_uartEmulatorHandle()
{
    //send data
    if(uartEnulator_txBufIndex)
    {
        Task_uartEmulator_txHandle();
        uartEnulator_txBufIndex = 0;
    }
    //rx data
    if(UART_RX_STATE_WAIT_FOR_GET == uart_rxState)
    {
        Task_uartEmulator_rxHandle();
        uart_rxState = UART_RX_STATE_EMPTY;
    }
}

extern void Driver_uartEmulator_init(uint32_t baudRate)
{
    isUartEmulatorInited = 1;
    // Initialize the Sensor Controller
    scifOsalInit();
    scifOsalRegisterCtrlReadyCallback(scCtrlReadyCallback);
    scifOsalRegisterTaskAlertCallback(scTaskAlertCallback);
    scifInit(&scifDriverSetup);

    // Start the UART emulator
    scifExecuteTasksOnceNbl(BV(SCIF_UART_EMULATOR_TASK_ID));

    // Enable baud rate generation
    scifUartSetBaudRate(baudRate);

    // Enable RX (10 idle bit periods required before enabling start bit detection)
    scifUartSetRxFifoThr(SCIF_UART_RX_FIFO_MAX_COUNT);
    scifUartSetRxTimeout(10 * 2);
    scifUartSetRxEnableReqIdleCount(10 * 2);
    scifUartRxEnable(1);

    // Enable events (half full RX FIFO or 10 bit period timeout
    scifUartSetEventMask(BV_SCIF_UART_ALERT_RX_FIFO_ABOVE_THR | BV_SCIF_UART_ALERT_RX_BYTE_TIMEOUT);
}

void Task_uartEmulator_txHandle()
{
    //采用多字节发送的方式
    for(uint32_t startPoint = 0; startPoint < uartEnulator_txBufIndex; )
    {
        if(uartEnulator_txBufIndex - startPoint > 50)
        {
            if(scifUartGetTxFifoCount())
            {
                //发送缓存还有数据未发完，暂时先不发
            }
            else
            {
                scifUartTxPutChars(uartEnulator_txBuf + startPoint, 50);
                startPoint += 50;
            }
        }
        else
        {
            if(scifUartGetTxFifoCount())
            {
                //发送缓存还有数据未发完，暂时先不发
            }
            else
            {
                scifUartTxPutChars(uartEnulator_txBuf + startPoint, uartEnulator_txBufIndex - startPoint);
                startPoint += (uartEnulator_txBufIndex - startPoint);
            }
        }
    }
}

void Task_uartEmulator_rxHandle()
{
    // Clear the ALERT interrupt source
    scifClearAlertIntSource();

    //recieve buf
    int rxFifoCount = scifUartGetRxFifoCount();
    scifUartRxGetChars(uartEnulator_rxBuf, rxFifoCount);

    //send to upper
    if(NULL != uartEmulator_recieveToUp)
        uartEmulator_recieveToUp((uint8_t*)uartEnulator_rxBuf, rxFifoCount);

    //echo test
#ifdef  UART_EMULATOR_ECHO_TEST_ON
    Driver_uartEmulator_sendChars((uint8_t*)uartEnulator_rxBuf, rxFifoCount);
#endif

    // Clear the events that triggered this
    scifUartClearEvents();

    // Acknowledge the alert event
    scifAckAlertEvents();
}

/**
 * uartEmulator_sendToDown
 *
 * ret:
 */
SendRet Driver_uartEmulator_sendChars(uint8_t* data, uint16_t len)
{
    SendRet sendRet = SEND_RET_OK;
    if(SEND_RET_OK == sendRet)
    {
        //是否初始化检查
        if(!isUartEmulatorInited)
            sendRet = SEND_RET_NOT_INIT;
    }

    if(SEND_RET_OK == sendRet)
    {
        //长度合法性检查
        if(uartEnulator_txBufIndex + len > UART_EMULATOR_TX_LEN)
        {
            sendRet = SEND_RET_TX_BUSY;
        }
    }

    if(SEND_RET_OK == sendRet)
    {   //检查通过，发送
        memcpy(uartEnulator_txBuf + uartEnulator_txBufIndex, data, len);
        uartEnulator_txBufIndex += len;

        // Wake up the OS task
        MyEvent_post( MY_UARTEMULATRO_EVT );
    }

    return sendRet;
}

void Driver_uartEmulator_registeRecieve(void (*func)(uint8_t* data, uint16_t len))
{
    uartEmulator_recieveToUp = func;
}
