/*
 * TestProcess_card.c
 *
 *  Created on: 2018��9��13��
 *      Author: zheng
 */
#include "TestProcess_card.h"
#include "TaskTest.h"
#include "Trace.h"

#define TIMEOUT_TEST_PROCESS_CARD       60000//60��

RET_TEST_CARD TestProcess_card()
{
    RET_TEST_CARD ret=RET_TEST_CARD_SUCCESS;

    //����֮ǰ���¼�
    TestEvent_pend(EVENT_TESTPROCESS_CARD, 1);
    TRACE_DEBUG("<<<<<<�ȴ�ˢ������...\n");

    //�ȴ�ˢ����Ϣ10����
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CARD, TIMEOUT_TEST_PROCESS_CARD);
    if (events & EVENT_TESTPROCESS_CARD)
    {
        ret = RET_TEST_CARD_SUCCESS;
        TRACE_DEBUG("ˢ���ɹ�.\n");
    }
    else
    {
        ret = RET_TEST_CARD_ERROR;
        TRACE_DEBUG("�ȴ�ˢ��%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_CARD);
    }
    return ret;
}
