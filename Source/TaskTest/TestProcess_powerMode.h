/*
 * TestProcess_powerMode.h
 *
 *  Created on: 2018Äê9ÔÂ28ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_POWERMODE_H_
#define SOURCE_TASKTEST_TESTPROCESS_POWERMODE_H_


#include "DataType.h"
#include "stdint.h"
typedef enum{
    RET_TEST_POWER_MODE_SUCCESS           = 0,
    RET_TEST_POWER_MODE_ERROR             = 1,
}RET_TEST_POWER_MODE;

RET_TEST_POWER_MODE TestProcess_powerMode(POWER_MODE powerMode);
void SetPin_lowPower();


#endif /* SOURCE_TASKTEST_TESTPROCESS_POWERMODE_H_ */
