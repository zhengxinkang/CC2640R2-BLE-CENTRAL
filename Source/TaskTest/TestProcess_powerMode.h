/*
 * TestProcess_powerMode.h
 *
 *  Created on: 2018��9��28��
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_POWERMODE_H_
#define SOURCE_TASKTEST_TESTPROCESS_POWERMODE_H_


#include "DataType.h"
#include "stdint.h"
typedef enum{
    RET_TEST_POWER_MODE_MIN_SUCCESS             = 0,
    RET_TEST_POWER_MODE_MIN_TOO_SMALL           = 1,    //��С���� ̫С�ˣ��쳣
    RET_TEST_POWER_MODE_MIN_SLIGHTLY_LARGER     = 2,    //��С���� �Դ�Լ��׼
    RET_TEST_POWER_MODE_MIN_POWER_OVERFLOW      = 99,   //�����ܶ�
}RET_TEST_POWER_MODE_MIN;

uint8_t TestProcess_powerModeMin(uint8_t minUaTooSmall, uint8_t minUa, uint8_t preMinUa);
void SetPin_lowPower();


#endif /* SOURCE_TASKTEST_TESTPROCESS_POWERMODE_H_ */
