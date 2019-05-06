/*
 * TestProcess_led.c
 *
 *  Created on: 2018年9月28日
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
#define TIMEOUT_TEST_PROCESS_LED        1000//10秒

RET_TEST_LED TestProcess_led()
{
    RET_TEST_LED ret = RET_TEST_LED_SUCCESS;

    TRACE_DEBUG("<<<<<<等待led测试结果...\n看到所有led闪烁请按下成功键，否则请按下失败键\n");

    //等待按键消息10秒钟
    for(uint8_t i = 0; i<MAX_TIMES_LED_CHECK; i++)
    {
        Task_sleep((200*1000)/Clock_tickPeriod);
        //消除之前的事件
        TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, 1);

        //发送led动作
        uint8_t para = ACTION_TYPE_LED;
        SendCmd_qcTest_action(&para, 1);

        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, TIMEOUT_TEST_PROCESS_LED);
        if (events & EVENT_TESTPROCESS_CONFIRM_SUCCESS)
        {
            ret = RET_TEST_LED_SUCCESS;
            TRACE_DEBUG("led确认为“成功”.\n");
            break;
        }
        else if (events & EVENT_TESTPROCESS_CONFIRM_FAIL)
        {
            ret = RET_TEST_LED_ERROR;
            TRACE_DEBUG("led确认为“失败”.\n");
            break;
        }
        else
        {
            ret = RET_TEST_LED_ERROR;
    //        TRACE_DEBUG("led确认%dms后超时！\n",TIMEOUT_TEST_PROCESS_LED);
        }
    }
    return ret;
}



