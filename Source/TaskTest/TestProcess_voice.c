/*
 * TestProcess_voice.c
 *
 *  Created on: 2018��9��28��
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TestProcess_voice.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "DataType.h"
#include "TaskTest.h"
#include "Trace.h"

#define MAX_TIMES_VOICE_CHECK               100
#define TIMEOUT_TEST_PROCESS_VOICE          2000//10��

RET_TEST_VOICE TestProcess_voice()
{
    RET_TEST_VOICE ret = RET_TEST_VOICE_SUCCESS;

    TRACE_DEBUG("<<<<<<�ȴ��������Խ��...\n���������밴�³ɹ�����û�����������밴��ʧ�ܼ�\n");

    for(uint8_t i = 0; i<MAX_TIMES_VOICE_CHECK; i++)
    {
        //����֮ǰ���¼�
        TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, 1);
        //������������
        uint8_t para = ACTION_TYPE_VOICE;
        SendCmd_qcTest_action(&para, 1);
        //�ȴ�������Ϣ10����
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, TIMEOUT_TEST_PROCESS_VOICE);
        if (events & EVENT_TESTPROCESS_CONFIRM_SUCCESS)
        {
            ret = RET_TEST_VOICE_SUCCESS;
            TRACE_DEBUG("����ȷ��Ϊ���ɹ���.\n");
            break;
        }
        else if (events & EVENT_TESTPROCESS_CONFIRM_FAIL)
        {
            ret = RET_TEST_VOICE_ERROR;
            TRACE_DEBUG("����ȷ��Ϊ��ʧ�ܡ�.\n");
            break;
        }
        else
        {
            ret = RET_TEST_VOICE_ERROR;
//            TRACE_DEBUG("����ȷ��%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_VOICE);
        }
    }
    return ret;
}
