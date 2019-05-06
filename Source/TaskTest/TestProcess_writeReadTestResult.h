/*
 * TestProcess_writeReadTestResult.h
 *
 *  Created on: 2018Äê10ÔÂ8ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_WRITEREADTESTRESULT_H_
#define SOURCE_TASKTEST_TESTPROCESS_WRITEREADTESTRESULT_H_


#include "TaskTest.h"
#include "stdint.h"

typedef enum{
    RET_TEST_WRITEREAD_TESTRESULT_SUCCESS                = 0,
    RET_TEST_WRITEREAD_TESTRESULT_ERROR                  = 1,
}RET_TEST_WRITEREAD_TESTRESULT;


RET_TEST_WRITEREAD_TESTRESULT TestProcess_writeTestResult(char result);
RET_TEST_WRITEREAD_TESTRESULT TestProcess_readTestResult(char* result);
RET_TEST_WRITEREAD_TESTRESULT TestProcess_clearTestResult();


#endif /* SOURCE_TASKTEST_TESTPROCESS_WRITEREADTESTRESULT_H_ */
