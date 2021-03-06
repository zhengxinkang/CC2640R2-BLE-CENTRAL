/*
 * TestProcess_select.c
 *
 *  Created on: 2018年10月12日
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TestProcess_select.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "DataType.h"
#include "TaskTest.h"
#include "Trace.h"

#define TIMEOUT_TEST_PROCESS_SELECT          20000//20秒

RET_TEST_SELECT TestProcess_select()
{
    RET_TEST_SELECT ret = RET_TEST_SELECT_SUCCESS;

    //消除之前的事件
    TestEvent_pend(EVENT_START_TEST|EVENT_TESTPROCESS_CONFIRM_SUCCESS, 1);

    //等待按键消息20秒钟
    uint32_t events = TestEvent_pend(EVENT_START_TEST|EVENT_TESTPROCESS_CONFIRM_SUCCESS, TIMEOUT_TEST_PROCESS_SELECT);
    if (events & EVENT_START_TEST)
    {
        ret = RET_TEST_SELECT_SUCCESS;
        TRACE_CODE("确认为“重测”.\n");
    }
    else if (events & EVENT_TESTPROCESS_CONFIRM_SUCCESS)
    {
        ret = RET_TEST_SELECT_ERROR;
        TRACE_CODE("确认为“退出”.\n");
    }
    else
    {
        ret = RET_TEST_SELECT_ERROR;
        TRACE_CODE("确认%dms后超时！\n",TIMEOUT_TEST_PROCESS_SELECT);
    }
    return ret;
}
