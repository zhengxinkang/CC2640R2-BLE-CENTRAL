/*
 * TestProcess_self.c
 *
 *  Created on: 2018年9月28日
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
    //发送开始自测指令----------------------------------
    SendCmd_qcTest_selfTest(NULL, 0);

    //等待接收返回消息
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_SELF, TIMEOUT_TEST_PROCESS_SELF);
    if (events & EVENT_TESTPROCESS_SELF)
    {
        ret = RET_TEST_SELF_SUCCESS;
        TRACE_CODE("自测结束.\n");
    }
    else
    {
        ret = RET_TEST_SELF_ERROR;
        TRACE_CODE("自测%dms后超时！\n",TIMEOUT_TEST_PROCESS_SELF);
    }
    return ret;
}
