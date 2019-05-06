/*
 * Uart_adapter.h
 *
 *  Created on: 2017Äê11ÔÂ1ÈÕ
 *      Author: Administrator
 */

#ifndef _UART_ADAPTER_H_
#define _UART_ADAPTER_H_


#include <stdint.h>
#define PORT_1  1
#define PORT_2  2
void Uart_adapter_send(uint8_t *buf, uint8_t len, uint8_t portNum);
void Uart_adapter_recieve(uint8_t* data, uint16_t len);


#endif /* _UART_ADAPTER_H_ */
