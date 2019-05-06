/*
 * TestProcess_keyBoard.c
 *
 *  Created on: 2018��9��13��
 *      Author: zheng
 */
#include "TestProcess_keyBoard.h"
#include "TaskTest.h"
#include "stdbool.h"

#define NUM_KEY                         12
#define MAX_TIMES_KEY_TEST              50
#define TIMEOUT_TEST_PROCESS_KEYBOARD   10000//10��

RET_TEST_KEYBOARD TestProcess_keyBoard()
{
    RET_TEST_KEYBOARD ret;
    uint8_t keyTable[NUM_KEY]={0,0,0,0,0,0,0,0,0,0,0,0};
    bool keyFinish = true;
    //���ͽ�����Լ���ģʽ
    ///TBD:
    //�ȴ�������Ϣ
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CARD, TIMEOUT_TEST_PROCESS_KEYBOARD);

    for(uint8_t i=0; i<MAX_TIMES_KEY_TEST; i++)
    {
        TRACE_DEBUG("��ʣ����Щ����δ����:\n");
        for(i=0;i<12;i++)
        {
            if(keyTable[i])
                TRACE_DEBUG("\t ", i);
            else
                TRACE_DEBUG("\t%d", i);

            if(0 == (i+1)%3)
                TRACE_DEBUG("\n");
        }
        //�ȴ�������Ϣ10����
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_KEYBOARD, TIMEOUT_TEST_PROCESS_KEYBOARD);
        if(EVENT_TESTPROCESS_KEYBOARD == events)
        {
            uint8_t keySerial = 0xff;
            //��ȡ���̼���
            //TBD:keySerial=
            if(0xff != keySerial)
                keyTable[keySerial]=1;
        }
        //�ж��Ƿ���ɼ��̼��
        for(uint8_t times = 0;times < NUM_KEY;times++)
        {
            if(0 == keyTable[times])
                keyFinish = false;
        }
    }
    return ret;
}
