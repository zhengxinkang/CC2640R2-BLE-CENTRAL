/*
 * TestProcess_card.c
 *
 *  Created on: 2018��9��13��
 *      Author: zheng
 */
#include "TestProcess_card.h"
#include "TaskTest.h"

#define TIMEOUT_TEST_PROCESS_CARD       10000//10��

RET_TEST_CARD TestProcess_card()
{
    RET_TEST_CARD ret;
    //���ͽ���ˢ��ģʽ
    ///TBD:
    //�ȴ�ˢ����Ϣ10����
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CARD, TIMEOUT_TEST_PROCESS_CARD);

    return ret;
}
