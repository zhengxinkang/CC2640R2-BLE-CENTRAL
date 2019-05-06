/*
 * TestProcess_voice.c
 *
 *  Created on: 2018��9��28��
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "BF_Util.h"
#include "TestProcess_voice.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "DataType.h"
#include "TaskTest.h"
#include "Trace.h"

#define MAX_TIMES_VOICE_CHECK               3
#define TIMES_VOICE_CHECK                   2
#define TIMEOUT_TEST_PROCESS_VOICE          500
#define TIMEOUT_ENVIROMENT_VOICE            1000

RET_TEST_VOICE TestProcess_voice()
{
    RET_TEST_VOICE ret = RET_TEST_VOICE_SUCCESS;
    uint8_t times_voice_check = 0;
//    TRACE_DEBUG("<<<<<<�ȴ��������Խ��...\n���������밴�³ɹ�����û�����������밴��ʧ�ܼ�\n");

    for(uint8_t i = 0; i<MAX_TIMES_VOICE_CHECK; i++)
    {
        //ȷ�ϻ����������ᴥ���������� �ɼ�1���ڵ�����
        //����֮ǰ���¼�
        TestEvent_pend(EVENT_TESTPROCESS_EVENT_VOICE, 1);
        BF_taskSleepMs(1000);
//        //������������
//        uint8_t para = ACTION_TYPE_VOICE;
//        SendCmd_qcTest_action(&para, 1);
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_EVENT_VOICE, TIMEOUT_TEST_PROCESS_VOICE);
        if (events & EVENT_TESTPROCESS_EVENT_VOICE)
        {
            ret = RET_TEST_VOICE_ENVIRONMENT_ERROR;
            TRACE_MARK("������������ȷ��Ϊ��ʧ�ܡ�����Χ��������̫��\n");
            continue;
        }
        else
        {
            ret = RET_TEST_VOICE_SUCCESS;
            TRACE_MARK("������������ȷ�ϳɹ���\n",TIMEOUT_TEST_PROCESS_VOICE);
        }

        //������������
        uint8_t para = ACTION_TYPE_VOICE;
        SendCmd_qcTest_action(&para, 1);
        //�ȴ�������Ϣ10����
        events = TestEvent_pend(EVENT_TESTPROCESS_EVENT_VOICE, TIMEOUT_TEST_PROCESS_VOICE);
        if (events & EVENT_TESTPROCESS_EVENT_VOICE)
        {
            times_voice_check++;
            TRACE_MARK("����ȷ��Ϊ���ɹ���,��%d��.\n", times_voice_check);
        }
        else
        {
            ret = RET_TEST_VOICE_ERROR;
            TRACE_MARK("����ȷ��%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_VOICE);
        }

        if(times_voice_check >= TIMES_VOICE_CHECK)
        {
            ret = RET_TEST_VOICE_SUCCESS;
            break;
        }

        //�ȴ���������
        BF_taskSleepMs(800);
    }
    return ret;
}
