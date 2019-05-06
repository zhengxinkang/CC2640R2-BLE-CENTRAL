/*
 * BuzzPort.h
 *
 *  Created on: 2017Äê8ÔÂ23ÈÕ
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_DRIVERS_BUZZPORT_BUZZPORT_H_
#define SOURCE_DRIVERS_BUZZPORT_BUZZPORT_H_


#include "stdint.h"
void BuzzPort_init(uint32_t frequency, uint8_t dutyPercent);
void BuzzPort_on();
void BuzzPort_off();
void BuzzPort_change(uint32_t frequency, uint8_t dutyPercent);


#endif /* SOURCE_DRIVERS_BUZZPORT_BUZZPORT_H_ */
