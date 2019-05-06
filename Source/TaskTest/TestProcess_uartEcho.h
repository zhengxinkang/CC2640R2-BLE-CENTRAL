/*
 * TestProcess_uartEcho.h
 *
 *  Created on: 2018Äê9ÔÂ27ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_UARTECHO_H_
#define SOURCE_TASKTEST_TESTPROCESS_UARTECHO_H_


#include "stdint.h"
#define TEST_STRING "HELLOWORLD!"

typedef enum{
    RET_TEST_UARTECHO_SUCCESS                = 0,
    RET_TEST_UARTECHO_ERROR                  = 1,
}RET_TEST_UARTECHO;

RET_TEST_UARTECHO TestProcess_uartEcho();


#endif /* SOURCE_TASKTEST_TESTPROCESS_UARTECHO_H_ */
