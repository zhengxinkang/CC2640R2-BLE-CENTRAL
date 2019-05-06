/*
 * TestProcess_card.c
 *
 *  Created on: 2018年9月13日
 *      Author: zheng
 */
#include "TestProcess_card.h"
#include "TaskTest.h"
#include "Trace.h"

#define TIMEOUT_TEST_PROCESS_CARD       60000//60秒

RET_TEST_CARD TestProcess_card()
{
    RET_TEST_CARD ret=RET_TEST_CARD_SUCCESS;

    //消除之前的事件
    TestEvent_pend(EVENT_TESTPROCESS_CARD, 1);
    TRACE_DEBUG("<<<<<<等待刷卡测试...\n");

    //等待刷卡消息10秒钟
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CARD, TIMEOUT_TEST_PROCESS_CARD);
    if (events & EVENT_TESTPROCESS_CARD)
    {
        ret = RET_TEST_CARD_SUCCESS;
        TRACE_DEBUG("刷卡成功.\n");
    }
    else
    {
        ret = RET_TEST_CARD_ERROR;
        TRACE_DEBUG("等待刷卡%dms后超时！\n",TIMEOUT_TEST_PROCESS_CARD);
    }
    return ret;
}
