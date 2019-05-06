/*
 * Driver_uart.h
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_DRIVERS_DRIVER_UART_H_
#define SOURCE_DRIVERS_DRIVER_UART_H_


#include "stdint.h"

#define MAX_NUM_UART_READ   64
/*********************************************************************
 * TYPEDEFS
 */
typedef void (*uartProtocol_CB_t)(uint8_t *data, uint16_t bytes);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
void Driver_uart_init(uint32_t bps, uartProtocol_CB_t uartProCallback);
void Driver_uart_send(const uint8_t *data, uint8_t len);
void Driver_uart_close();
void Driver_uart_open();

#endif /* SOURCE_DRIVERS_DRIVER_UART_H_ */
