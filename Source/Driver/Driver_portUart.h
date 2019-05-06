/*
 * Driver_portUart.h
 *
 *  Created on: 2018Äê8ÔÂ16ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_DRIVERS_DRIVER_PORTUART_H_
#define SOURCE_DRIVERS_DRIVER_PORTUART_H_


#include "Types.h"
#include "ConfigProtocol.h"
#if (BLE_UART_MODE == BLE_UART_MODE_SLEEP)
uint8_t Driver_uartGpio_open();
uint8_t Driver_uartGpio_close();
uint8_t Driver_uartGpioSetLow();
uint8_t Driver_uartGpioSetHigh();
#endif//BLE_UART_MODE


#endif /* SOURCE_DRIVERS_DRIVER_PORTUART_H_ */
