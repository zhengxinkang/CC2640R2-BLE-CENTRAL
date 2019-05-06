/*
 * TestProcess_uartEcho.c
 *
 *  Created on: 2018年9月27日
 *      Author: zheng
 */
#include "TestProcess_uartEcho.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "TaskTest.h"
#include "Trace.h"
#include "BF_Util.h"

#define MAX_TIMES_UART_ECHO_CHECK               5
#define TIMES_UART_ECHO_CHECK                   3
#define TIMEOUT_TEST_PROCESS_UARTECHO           500


RET_TEST_UARTECHO TestProcess_uartEcho()
{
    RET_TEST_UARTECHO ret;
    uint8_t checkSuccessTimes = 0;
    uint8_t checkFailTimes = 0;

    for(uint8_t times=0; times<MAX_TIMES_UART_ECHO_CHECK; times++)
    {
        //发送回声指令----------------------------------
        BF_taskSleepMs(100);
        SendCmd_qcTest_echo(TEST_STRING, sizeof(TEST_STRING));
        //等待接收回声消息
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_UART_ECHO, TIMEOUT_TEST_PROCESS_UARTECHO);
        if (events & EVENT_TESTPROCESS_UART_ECHO)
        {
            ret = RET_TEST_UARTECHO_SUCCESS;
            TRACE_CODE("UART回声测试成功.\n");
            checkSuccessTimes++;
        }
        else
        {
            ret = RET_TEST_UARTECHO_ERROR;
            TRACE_CODE("等待UART回声测试%dms后超时！\n",TIMEOUT_TEST_PROCESS_UARTECHO);
            checkFailTimes++;
        }

        if(checkSuccessTimes >= TIMES_UART_ECHO_CHECK)
        {
            ret = RET_TEST_UARTECHO_SUCCESS;
            break;
        }
        if(times >= MAX_TIMES_UART_ECHO_CHECK-1)
        {
            ret = RET_TEST_UARTECHO_ERROR;
            break;
        }
    }
    return ret;
}

//检测是否进入测试模式成功
RET_TEST_UARTECHO TestProcess_uartEchoCheck()
{
    RET_TEST_UARTECHO ret;
    uint8_t checkSuccessTimes = 0;
    uint8_t checkFailTimes = 0;

    for(uint8_t times=0; times<MAX_TIMES_UART_ECHO_CHECK; times++)
    {
        //发送回声指令----------------------------------
        SendCmd_qcTest_echo(TEST_STRING, sizeof(TEST_STRING));
        //等待接收回声消息
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_UART_ECHO, TIMEOUT_TEST_PROCESS_UARTECHO);
        if (events & EVENT_TESTPROCESS_UART_ECHO)
        {
            ret = RET_TEST_UARTECHO_SUCCESS;
            TRACE_CODE("UART回声测试成功.\n");
            checkSuccessTimes++;
        }
        else
        {
            ret = RET_TEST_UARTECHO_ERROR;
            TRACE_CODE("等待UART回声测试%dms后超时！\n",TIMEOUT_TEST_PROCESS_UARTECHO);
            checkFailTimes++;
        }

        if(checkSuccessTimes >= 1)
        {
            ret = RET_TEST_UARTECHO_SUCCESS;
            break;
        }
        if(times >= MAX_TIMES_UART_ECHO_CHECK-1)
        {
            ret = RET_TEST_UARTECHO_ERROR;
            break;
        }
    }
    return ret;
}
