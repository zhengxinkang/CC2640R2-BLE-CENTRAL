/*
 * TestProcess_self.h
 *
 *  Created on: 2018Äê9ÔÂ28ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_SELF_H_
#define SOURCE_TASKTEST_TESTPROCESS_SELF_H_


#include "TaskTest.h"
#include "stdint.h"

typedef enum{
    RET_TEST_SELF_SUCCESS                   = 0,
    RET_TEST_SELF_ERROR                     = 1,
}RET_TEST_SELF;

RET_TEST_SELF   TestProcess_self();
void            TestProcess_selfSetResult(uint8_t* para, uint8_t paraLen);
uint8_t*        TestProcess_selfGetResult(uint8_t* len);


#endif /* SOURCE_TASKTEST_TESTPROCESS_SELF_H_ */
