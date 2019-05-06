/*
 * TestProcess_card.c
 *
 *  Created on: 2018年9月13日
 *      Author: zheng
 */
#include "TestProcess_card.h"
#include "TaskTest.h"

#define TIMEOUT_TEST_PROCESS_CARD       10000//10秒

RET_TEST_CARD TestProcess_card()
{
    RET_TEST_CARD ret;
    //发送进入刷卡模式
    ///TBD:
    //等待刷卡消息10秒钟
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CARD, TIMEOUT_TEST_PROCESS_CARD);

    return ret;
}
