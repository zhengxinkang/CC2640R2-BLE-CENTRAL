/*
 * TestProcess_select.h
 *
 *  Created on: 2018Äê10ÔÂ12ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_SELECT_H_
#define SOURCE_TASKTEST_TESTPROCESS_SELECT_H_


#include "stdint.h"
typedef enum{
    RET_TEST_SELECT_SUCCESS           = 0,
    RET_TEST_SELECT_ERROR             = 1,
}RET_TEST_SELECT;

RET_TEST_SELECT TestProcess_select();


#endif /* SOURCE_TASKTEST_TESTPROCESS_SELECT_H_ */
