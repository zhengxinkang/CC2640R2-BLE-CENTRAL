/*
 * TestProcess_powerMode.h
 *
 *  Created on: 2018年9月28日
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_POWERMODE_H_
#define SOURCE_TASKTEST_TESTPROCESS_POWERMODE_H_


#include "DataType.h"
#include "stdint.h"
typedef enum{
    RET_TEST_POWER_MODE_MIN_SUCCESS             = 0,
    RET_TEST_POWER_MODE_MIN_TOO_SMALL           = 1,    //最小电流 太小了，异常
    RET_TEST_POWER_MODE_MIN_SLIGHTLY_LARGER     = 2,    //最小电流 略大约标准
    RET_TEST_POWER_MODE_MIN_POWER_OVERFLOW      = 99,   //超过很多
}RET_TEST_POWER_MODE_MIN;

uint8_t TestProcess_powerModeMin(uint8_t minUaTooSmall, uint8_t minUa, uint8_t preMinUa);
void SetPin_lowPower();


#endif /* SOURCE_TASKTEST_TESTPROCESS_POWERMODE_H_ */
