/*
 * UartAdapter.h
 *
 *  Created on: 2017��8��9��
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_CONSOLE_UARTADAPTER_H_
#define SOURCE_CONSOLE_UARTADAPTER_H_

//******************************************************************
// ��Դ���壬�����ﶨ�屾������ʹ�õ������Դ
#include "Hal_UartEmulator.h"

extern void Hal_uartEmulator_printf(const uint8_t *pcString, ...);
extern void Hal_uartEmulator_write(uint8_t *pcBuf, uint16_t ulLen);
extern void Hal_uartEmulator_registeRecieve( void (*func)(uint8_t* data, uint16_t len) );

#endif /* SOURCE_CONSOLE_UARTADAPTER_H_ */
