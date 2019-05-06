/*
 * UartDriver.h
 *
 *  Created on: 2017年8月9日
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_CONSOLE_STRLINE_H_
#define SOURCE_CONSOLE_STRLINE_H_


#include <string.h>
#include <stdint.h>

void StrLine_init(void);
void StrLine_recieveToUp_setFunc(void (*func)(uint8_t* data, uint16_t len));
void StrLine_WriteData(uint8_t* data, uint16_t Len);


#endif /* SOURCE_CONSOLE_STRLINE_H_ */
