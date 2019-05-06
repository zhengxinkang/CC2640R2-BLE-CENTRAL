/*
 * TestProcess_card.c
 *
 *  Created on: 2018年9月13日
 *      Author: zheng
 */
#include "TestProcess_card.h"
#include "Lock_atcion.h"
#include "TaskTest.h"
#include "Trace.h"
#include "BF_Util.h"

#define TIMEOUT_TEST_PROCESS_CARD       1000
#define MAX_TIMES_CARD_TRY              2

RET_TEST_CARD TestProcess_card()
{
    RET_TEST_CARD ret=RET_TEST_CARD_SUCCESS;
    //消除之前的事件
    TestEvent_pend(EVENT_TESTPROCESS_CARD, 1);

    for(uint8_t time=0; time<MAX_TIMES_CARD_TRY; time++)
    {
        BF_taskSleepMs(500);
        Lock_action_cardActive(1000);

        //等待刷卡消息1秒钟
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CARD, TIMEOUT_TEST_PROCESS_CARD);
        if (events & EVENT_TESTPROCESS_CARD)
        {
            ret = RET_TEST_CARD_SUCCESS;
            TRACE_CODE("刷卡成功.\n");
            break;
        }
        else
        {
            ret = RET_TEST_CARD_ERROR;
            TRACE_DEBUG("等待刷卡%dms后超时！\n",TIMEOUT_TEST_PROCESS_CARD);
        }
    }

    return ret;
}
