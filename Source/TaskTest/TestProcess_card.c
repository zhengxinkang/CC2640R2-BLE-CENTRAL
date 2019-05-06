/*
 * TestProcess_card.c
 *
 *  Created on: 2018��9��13��
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
    //����֮ǰ���¼�
    TestEvent_pend(EVENT_TESTPROCESS_CARD, 1);

    for(uint8_t time=0; time<MAX_TIMES_CARD_TRY; time++)
    {
        BF_taskSleepMs(500);
        Lock_action_cardActive(1000);

        //�ȴ�ˢ����Ϣ1����
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CARD, TIMEOUT_TEST_PROCESS_CARD);
        if (events & EVENT_TESTPROCESS_CARD)
        {
            ret = RET_TEST_CARD_SUCCESS;
            TRACE_CODE("ˢ���ɹ�.\n");
            break;
        }
        else
        {
            ret = RET_TEST_CARD_ERROR;
            TRACE_DEBUG("�ȴ�ˢ��%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_CARD);
        }
    }

    return ret;
}
