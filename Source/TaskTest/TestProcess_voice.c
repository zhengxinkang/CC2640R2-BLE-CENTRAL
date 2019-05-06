/*
 * TestProcess_voice.c
 *
 *  Created on: 2018年9月28日
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
//    TRACE_DEBUG("<<<<<<等待语音测试结果...\n听到语音请按下成功键，没有听到语音请按下失败键\n");

    for(uint8_t i = 0; i<MAX_TIMES_VOICE_CHECK; i++)
    {
        //确认环境噪音不会触发语音测试 采集1秒内的噪音
        //消除之前的事件
        TestEvent_pend(EVENT_TESTPROCESS_EVENT_VOICE, 1);
        BF_taskSleepMs(1000);
//        //发送语音动作
//        uint8_t para = ACTION_TYPE_VOICE;
//        SendCmd_qcTest_action(&para, 1);
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_EVENT_VOICE, TIMEOUT_TEST_PROCESS_VOICE);
        if (events & EVENT_TESTPROCESS_EVENT_VOICE)
        {
            ret = RET_TEST_VOICE_ENVIRONMENT_ERROR;
            TRACE_MARK("语音环境干扰确认为“失败”，周围环境干扰太大。\n");
            continue;
        }
        else
        {
            ret = RET_TEST_VOICE_SUCCESS;
            TRACE_MARK("语音环境干扰确认成功。\n",TIMEOUT_TEST_PROCESS_VOICE);
        }

        //发送语音动作
        uint8_t para = ACTION_TYPE_VOICE;
        SendCmd_qcTest_action(&para, 1);
        //等待按键消息10秒钟
        events = TestEvent_pend(EVENT_TESTPROCESS_EVENT_VOICE, TIMEOUT_TEST_PROCESS_VOICE);
        if (events & EVENT_TESTPROCESS_EVENT_VOICE)
        {
            times_voice_check++;
            TRACE_MARK("语音确认为“成功”,第%d次.\n", times_voice_check);
        }
        else
        {
            ret = RET_TEST_VOICE_ERROR;
            TRACE_MARK("语音确认%dms后超时！\n",TIMEOUT_TEST_PROCESS_VOICE);
        }

        if(times_voice_check >= TIMES_VOICE_CHECK)
        {
            ret = RET_TEST_VOICE_SUCCESS;
            break;
        }

        //等待声音结束
        BF_taskSleepMs(800);
    }
    return ret;
}
