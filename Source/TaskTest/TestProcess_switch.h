/*
 * TestProcess_switch.h
 *
 *  Created on: 2018Äê9ÔÂ13ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_SWITCH_H_
#define SOURCE_TASKTEST_TESTPROCESS_SWITCH_H_


#include "stdint.h"
typedef enum{
    RET_TEST_SWITCH_SUCCESS                 = 0,
    RET_TEST_SWITCH_ERROR                   = 1,
}RET_TEST_SWITCH;

RET_TEST_SWITCH TestProcess_antiLock();
RET_TEST_SWITCH TestProcess_preventDismantle();
RET_TEST_SWITCH TestProcess_settingButton();
RET_TEST_SWITCH TestProcess_clearButton();
RET_TEST_SWITCH TestProcess_obliqueTongue();


#endif /* SOURCE_TASKTEST_TESTPROCESS_SWITCH_H_ */
