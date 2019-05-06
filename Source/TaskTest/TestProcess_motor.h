/*
 * TestProcess_motor.h
 *
 *  Created on: 2018Äê9ÔÂ12ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_MOTOR_H_
#define SOURCE_TASKTEST_TESTPROCESS_MOTOR_H_


#include "stdint.h"
typedef enum{
    RET_TEST_MOTOR_SUCCESS                  = 0,
    RET_TEST_MOTOR_ERROR                    = 1,
}RET_TEST_MOTOR;

RET_TEST_MOTOR TestProcess_motorForward();
RET_TEST_MOTOR TestProcess_motorReversal();


#endif /* SOURCE_TASKTEST_TESTPROCESS_MOTOR_H_ */
