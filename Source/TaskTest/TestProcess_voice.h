/*
 * TestProcess_voice.h
 *
 *  Created on: 2018��9��28��
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_VOICE_H_
#define SOURCE_TASKTEST_TESTPROCESS_VOICE_H_


#include "stdint.h"
typedef enum{
    RET_TEST_VOICE_SUCCESS           = 0,
    RET_TEST_VOICE_ERROR             = 1,
}RET_TEST_VOICE;

RET_TEST_VOICE TestProcess_voice();


#endif /* SOURCE_TASKTEST_TESTPROCESS_VOICE_H_ */
