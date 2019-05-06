/*
 * TestProcess_setInfo.h
 *
 *  Created on: 2018Äê9ÔÂ27ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_SETINFO_H_
#define SOURCE_TASKTEST_TESTPROCESS_SETINFO_H_


#include "TaskTest.h"
#include "TargetModel.h"
#include "stdint.h"

typedef enum{
    RET_TEST_SETINFO_SUCCESS                = 0,
    RET_TEST_SETINFO_ERROR                  = 1,
}RET_TEST_SETINFO;

extern uint8_t deviceInfoBuf[4];

RET_TEST_SETINFO TestProcess_setInfo(TARGET_MODEL targetModel);
RET_TEST_SETINFO TestProcess_readInfo(TARGET_MODEL targetModel);


#endif /* SOURCE_TASKTEST_TESTPROCESS_SETINFO_H_ */
