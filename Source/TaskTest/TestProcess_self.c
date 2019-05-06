/*
 * TestProcess_self.c
 *
 *  Created on: 2018��9��28��
 *      Author: zheng
 */
#include "TaskTest.h"
#include "TargetModel.h"
#include "TestProcess_self.h"
#include "Trace.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

#define TIMEOUT_TEST_PROCESS_SELF   500

RET_TEST_SELF TestProcess_self( )
{
    RET_TEST_SELF ret;
    Task_sleep((3000*1000)/Clock_tickPeriod);
    //���Ϳ�ʼ�Բ�ָ��----------------------------------
    SendCmd_qcTest_selfTest(NULL, 0);

    //�ȴ����շ�����Ϣ
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_SELF, TIMEOUT_TEST_PROCESS_SELF);
    if (events & EVENT_TESTPROCESS_SELF)
    {
        ret = RET_TEST_SELF_SUCCESS;
        TRACE_CODE("�Բ����.\n");
    }
    else
    {
        ret = RET_TEST_SELF_ERROR;
        TRACE_CODE("�Բ�%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_SELF);
    }
    return ret;
}
