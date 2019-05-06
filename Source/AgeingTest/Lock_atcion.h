/*
 * Lock_atcion.h
 *
 *  Created on: 2018Äê11ÔÂ23ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_AGEINGTEST_LOCK_ATCION_H_
#define SOURCE_AGEINGTEST_LOCK_ATCION_H_


#include "stdbool.h"
#include "stdint.h"
void Lock_action_init(void);
void Lock_action_bolt(uint16_t ms);
void Lock_action_buttonSet();
void Lock_action_buttonClr();
void Lock_action_relock(bool isLocked);
void Lock_action_fq(bool isActive);

void Lock_action_power(bool powerBat, bool powerUsb);
void Lock_action_cardActive(uint16_t ms);


#endif /* SOURCE_AGEINGTEST_LOCK_ATCION_H_ */
