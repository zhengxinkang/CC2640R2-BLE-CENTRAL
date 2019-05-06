/*
 * TestProcess_voice.c
 *
 *  Created on: 2018年9月28日
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
#define TIMEOUT_TEST_PROCESS_VOICE          2000//10秒

RET_TEST_VOICE TestProcess_voice()
{
    RET_TEST_VOICE ret = RET_TEST_VOICE_SUCCESS;

    TRACE_DEBUG("<<<<<<等待语音测试结果...\n听到语音请按下成功键，没有听到语音请按下失败键\n");

    for(uint8_t i = 0; i<MAX_TIMES_VOICE_CHECK; i++)
    {
        //消除之前的事件
        TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, 1);
        //发送语音动作
        uint8_t para = ACTION_TYPE_VOICE;
        SendCmd_qcTest_action(&para, 1);
        //等待按键消息10秒钟
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, TIMEOUT_TEST_PROCESS_VOICE);
        if (events & EVENT_TESTPROCESS_CONFIRM_SUCCESS)
        {
            ret = RET_TEST_VOICE_SUCCESS;
            TRACE_DEBUG("语音确认为“成功”.\n");
            break;
        }
        else if (events & EVENT_TESTPROCESS_CONFIRM_FAIL)
        {
            ret = RET_TEST_VOICE_ERROR;
            TRACE_DEBUG("语音确认为“失败”.\n");
            break;
        }
        else
        {
            ret = RET_TEST_VOICE_ERROR;
//            TRACE_DEBUG("语音确认%dms后超时！\n",TIMEOUT_TEST_PROCESS_VOICE);
        }
    }
    return ret;
}
