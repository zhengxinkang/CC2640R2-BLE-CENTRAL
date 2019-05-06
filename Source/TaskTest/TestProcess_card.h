/*
 * TestProcess_card.h
 *
 *  Created on: 2018Äê9ÔÂ13ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_CARD_H_
#define SOURCE_TASKTEST_TESTPROCESS_CARD_H_


#include "stdint.h"
typedef enum{
    RET_TEST_CARD_SUCCESS           = 0,
    RET_TEST_CARD_ERROR             = 1,
}RET_TEST_CARD;

RET_TEST_CARD TestProcess_card();


#endif /* SOURCE_TASKTEST_TESTPROCESS_CARD_H_ */
