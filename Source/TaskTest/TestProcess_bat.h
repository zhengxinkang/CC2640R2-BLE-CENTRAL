/*
 * TestProcess_bat.h
 *
 *  Created on: 2018Äê9ÔÂ13ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_BAT_H_
#define SOURCE_TASKTEST_TESTPROCESS_BAT_H_


#include "stdint.h"
typedef enum{
    RET_TEST_BAT_SUCCESS            = 0,
    RET_TEST_BAT_ERROR              = 1,
}RET_TEST_BAT;

RET_TEST_BAT TestProcess_bat();


#endif /* SOURCE_TASKTEST_TESTPROCESS_BAT_H_ */
