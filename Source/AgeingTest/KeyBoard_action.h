/*
 * KeyBoard_action.h
 *
 *  Created on: 2018Äê11ÔÂ23ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_AGEINGTEST_KEYBOARD_ACTION_H_
#define SOURCE_AGEINGTEST_KEYBOARD_ACTION_H_


#include "Hal_expandOutput.h"
uint8_t KeyBoard_wake(void);
uint8_t KeyBoard_stringAction(char* keyCodeString, uint16_t interval, uint16_t duration);
uint8_t KeyBoard_oneAction(EXPAND_OUTPUT_SERIAL serial);
uint8_t KeyBoard_fastSleep();


#endif /* SOURCE_AGEINGTEST_KEYBOARD_ACTION_H_ */
