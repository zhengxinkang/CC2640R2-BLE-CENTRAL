/*
 * TestProcess_led.h
 *
 *  Created on: 2018Äê9ÔÂ28ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_LED_H_
#define SOURCE_TASKTEST_TESTPROCESS_LED_H_


#include "stdint.h"
typedef enum{
    RET_TEST_LED_SUCCESS           = 0,
    RET_TEST_LED_ERROR             = 1,
}RET_TEST_LED;

RET_TEST_LED TestProcess_led();


#endif /* SOURCE_TASKTEST_TESTPROCESS_LED_H_ */
