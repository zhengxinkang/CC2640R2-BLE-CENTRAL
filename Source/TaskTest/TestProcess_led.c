/*
 * TestProcess_led.c
 *
 *  Created on: 2018��9��28��
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TestProcess_led.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "DataType.h"
#include "TaskTest.h"
#include "Trace.h"

#define MAX_TIMES_LED_CHECK             1000
#define TIMEOUT_TEST_PROCESS_LED        1000//10��

RET_TEST_LED TestProcess_led()
{
    RET_TEST_LED ret = RET_TEST_LED_SUCCESS;

    TRACE_DEBUG("<<<<<<�ȴ�led���Խ��...\n��������led��˸�밴�³ɹ����������밴��ʧ�ܼ�\n");

    //�ȴ�������Ϣ10����
    for(uint8_t i = 0; i<MAX_TIMES_LED_CHECK; i++)
    {
        Task_sleep((200*1000)/Clock_tickPeriod);
        //����֮ǰ���¼�
        TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, 1);

        //����led����
        uint8_t para = ACTION_TYPE_LED;
        SendCmd_qcTest_action(&para, 1);

        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, TIMEOUT_TEST_PROCESS_LED);
        if (events & EVENT_TESTPROCESS_CONFIRM_SUCCESS)
        {
            ret = RET_TEST_LED_SUCCESS;
            TRACE_DEBUG("ledȷ��Ϊ���ɹ���.\n");
            break;
        }
        else if (events & EVENT_TESTPROCESS_CONFIRM_FAIL)
        {
            ret = RET_TEST_LED_ERROR;
            TRACE_DEBUG("ledȷ��Ϊ��ʧ�ܡ�.\n");
            break;
        }
        else
        {
            ret = RET_TEST_LED_ERROR;
    //        TRACE_DEBUG("ledȷ��%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_LED);
        }
    }
    return ret;
}



