/*
 * TestProcess_switch.h
 *
 *  Created on: 2018��9��13��
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_SWITCH_H_
#define SOURCE_TASKTEST_TESTPROCESS_SWITCH_H_


#include "stdint.h"
typedef enum{
    RET_TEST_SWITCH_SUCCESS                 = 0,
    RET_TEST_SWITCH_ERROR                   = 1,
}RET_TEST_SWITCH;

RET_TEST_SWITCH TestProcess_switchAntiLock();
RET_TEST_SWITCH TestProcess_switchPreventDismantle();


#endif /* SOURCE_TASKTEST_TESTPROCESS_SWITCH_H_ */
