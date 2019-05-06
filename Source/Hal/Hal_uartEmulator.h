/*
 * Hal_uart.h
 *
 *  Created on: 2018Äê7ÔÂ5ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_HAL_HAL_UARTEMULATOR_H_
#define SOURCE_HAL_HAL_UARTEMULATOR_H_


#include <stdint.h>
//void Hal_uartEmulator_printf(const uint8_t *pcString, ...);
void Hal_uartEmulator_write(uint8_t *pcBuf, uint16_t ulLen);
void Hal_uartEmulator_registeRecieve( void (*func)(uint8_t* data, uint16_t len) );


#endif /* SOURCE_HAL_HAL_UARTEMULATOR_H_ */
