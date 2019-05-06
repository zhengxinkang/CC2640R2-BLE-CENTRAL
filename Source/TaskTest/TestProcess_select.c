/*
 * TestProcess_select.c
 *
 *  Created on: 2018��10��12��
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TestProcess_select.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "DataType.h"
#include "TaskTest.h"
#include "Trace.h"

#define TIMEOUT_TEST_PROCESS_SELECT          20000//20��

RET_TEST_SELECT TestProcess_select()
{
    RET_TEST_SELECT ret = RET_TEST_SELECT_SUCCESS;

    //����֮ǰ���¼�
    TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, 1);

    //�ȴ�������Ϣ20����
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, TIMEOUT_TEST_PROCESS_SELECT);
    if (events & EVENT_TESTPROCESS_CONFIRM_SUCCESS)
    {
        ret = RET_TEST_SELECT_SUCCESS;
        TRACE_CODE("ȷ��Ϊ���ɹ���.\n");
    }
    else if (events & EVENT_TESTPROCESS_CONFIRM_FAIL)
    {
        ret = RET_TEST_SELECT_ERROR;
        TRACE_CODE("ȷ��Ϊ��ʧ�ܡ�.\n");
    }
    else
    {
        ret = RET_TEST_SELECT_ERROR;
        TRACE_CODE("����ȷ��%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_SELECT);
    }
    return ret;
}
