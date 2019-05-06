/*
 * TestProcess_writeReadTestResult.c
 *
 *  Created on: 2018年10月8日
 *      Author: zheng
 */


#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TestProcess_writeReadTestResult.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "DataType.h"
#include "BF_Util.h"

#define TIMEOUT_TEST_PROCESS_WRITEREAD_TESTRESULT   1200
#define TIMEOUT_TEST_PROCESS_ERASE_TESTRESULT   4000

RET_TEST_WRITEREAD_TESTRESULT TestProcess_writeTestResult(char result)
{
    RET_TEST_WRITEREAD_TESTRESULT ret;
    //发送设置测试结果指令----------------------------------
    uint8_t para[2];
    para[0] = WRITEREAD_TESTRESULT_TYPE_WRITE;
    para[1] = result;
    //延迟100ms
    Task_sleep((100*1000)/Clock_tickPeriod);
    SendCmd_qcTest_writeReadTestResult(para, 2);

    //等待接收返回消息
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_WRITEREAD_TESTRESULT, TIMEOUT_TEST_PROCESS_WRITEREAD_TESTRESULT);
    if (events & EVENT_TESTPROCESS_WRITEREAD_TESTRESULT)
    {
        ret = RET_TEST_WRITEREAD_TESTRESULT_SUCCESS;
        TRACE_CODE("设置测试结果成功.\n");
    }
    else
    {
        ret = RET_TEST_WRITEREAD_TESTRESULT_ERROR;
        TRACE_DEBUG("设置测试结果%dms后超时！\n",TIMEOUT_TEST_PROCESS_WRITEREAD_TESTRESULT);
    }
    return ret;
}

RET_TEST_WRITEREAD_TESTRESULT TestProcess_readTestResult(char* result)
{
    RET_TEST_WRITEREAD_TESTRESULT ret;
    //发送获取测试结果指令----------------------------------
    BF_taskSleepMs(200);
    uint8_t para[2];
    para[0] = WRITEREAD_TESTRESULT_TYPE_READ;
    para[1] = 0x00;
    SendCmd_qcTest_writeReadTestResult(para, 2);

    //等待接收返回消息
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_WRITEREAD_TESTRESULT, TIMEOUT_TEST_PROCESS_WRITEREAD_TESTRESULT);
    if (events & EVENT_TESTPROCESS_WRITEREAD_TESTRESULT)
    {
        ret = RET_TEST_WRITEREAD_TESTRESULT_SUCCESS;
        TRACE_CODE("读取测试结果成功.\n");
        *result = testResult;
    }
    else
    {
        ret = RET_TEST_WRITEREAD_TESTRESULT_ERROR;
        TRACE_DEBUG("读取测试结果%dms后超时！\n",TIMEOUT_TEST_PROCESS_WRITEREAD_TESTRESULT);
    }
    return ret;
}

RET_TEST_WRITEREAD_TESTRESULT TestProcess_clearTestResult()
{
    RET_TEST_WRITEREAD_TESTRESULT ret;
    //发送获取测试结果指令----------------------------------
    BF_taskSleepMs(200);
    uint8_t para[2];
    para[0] = WRITEREAD_TESTRESULT_TYPE_CLEAR;
    para[1] = 0x00;
    SendCmd_qcTest_writeReadTestResult(para, 2);

    //等待接收返回消息
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_WRITEREAD_TESTRESULT, TIMEOUT_TEST_PROCESS_ERASE_TESTRESULT);
    if (events & EVENT_TESTPROCESS_WRITEREAD_TESTRESULT)
    {
        ret = RET_TEST_WRITEREAD_TESTRESULT_SUCCESS;
        TRACE_CODE("擦除测试结果成功.\n");
        BF_taskSleepMs(200);
    }
    else
    {
        ret = RET_TEST_WRITEREAD_TESTRESULT_ERROR;
        TRACE_DEBUG("擦除测试结果%dms后超时！\n",TIMEOUT_TEST_PROCESS_WRITEREAD_TESTRESULT);
    }
    return ret;
}

