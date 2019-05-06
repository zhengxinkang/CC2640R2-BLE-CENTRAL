/*
 * TestProcess_keyBoard.h
 *
 *  Created on: 2018Äê9ÔÂ13ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_KEYBOARD_H_
#define SOURCE_TASKTEST_TESTPROCESS_KEYBOARD_H_


#include "stdint.h"
typedef enum{
    RET_TEST_KEYBOARD_SUCCESS       = 0,
    RET_TEST_KEYBOARD_ERROR         = 1,
}RET_TEST_KEYBOARD;

RET_TEST_KEYBOARD TestProcess_keyBoard();


#endif /* SOURCE_TASKTEST_TESTPROCESS_KEYBOARD_H_ */
