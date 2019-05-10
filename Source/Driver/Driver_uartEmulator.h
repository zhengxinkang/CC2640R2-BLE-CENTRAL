/*
 * UartEmulator.h
 *
 */

#ifndef SOURCE_DRIVERS_UARTEMULATOR_TASK_UARTEMULATOR_H_
#define SOURCE_DRIVERS_UARTEMULATOR_TASK_UARTEMULATOR_H_

#include <stdint.h>

//#define UART_EMULATOR_ECHO_TEST_ON
#define UART_EMULATOR_RX_LEN        (SCIF_UART_RX_FIFO_MAX_COUNT+1)
#define UART_EMULATOR_TX_LEN        600

typedef enum
{
    UART_RX_STATE_EMPTY         =0,//空闲
    UART_RX_STATE_WAIT_FOR_GET  =1,//有数据等待处理
}Uart_Rx_State;

typedef enum
{
    SEND_RET_OK             = 0,
    SEND_RET_LEN_TO_LONG    = 1,
    SEND_RET_TX_BUSY        = 2,
    SEND_RET_NOT_INIT       = 3,
}SendRet;

extern void Driver_uartEmulator_init(uint32_t baudRate);
SendRet     Driver_uartEmulator_sendChars(uint8_t* data, uint16_t len);
void        Driver_uartEmulator_registeRecieve(void (*func)(uint8_t* data, uint16_t len));

extern void MyEvent_uartEmulatorHandle();


#endif /* SOURCE_DRIVERS_UARTEMULATOR_TASK_UARTEMULATOR_H_ */
