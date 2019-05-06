/*
 * TestProcess_motor.c
 *
 *  Created on: 2018年9月12日
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "TaskTest.h"
#include "TestProcess_motor.h"
#include "Driver_gpio.h"
#include "DataType.h"

#define MAX_TIMES_MOTOR_CHECK   4
#define TIMES_MOTOR_CHECK       3

#define TIMEOUT_TEST_PROCESS_MOTOREVENT   200

RET_TEST_MOTOR TestProcess_motorForward()
{
    RET_TEST_MOTOR ret;
    //检测电机正转结果
    Task_sleep((200*1000)/Clock_tickPeriod);
    uint8_t forwardCheckTimes=0;
    for(uint8_t times = 0; times < MAX_TIMES_MOTOR_CHECK; times++)
    {
        //发送电机反转动作---
        uint8_t para = ACTION_TYPE_MOTOR_REVERSE;
        SendCmd_qcTest_action(&para, 1);
        //延时
        Task_sleep((200*1000)/Clock_tickPeriod);
        //发送电机正转动作----------------------------------
        para = ACTION_TYPE_MOTOR_FORWARD;
        SendCmd_qcTest_action(&para, 1);
        //延时
        Task_sleep((200*1000)/Clock_tickPeriod);

        //等待接收返回消息
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_MOTOR_SW1, TIMEOUT_TEST_PROCESS_MOTOREVENT);
        if (events & EVENT_TESTPROCESS_MOTOR_SW1)
        {
            forwardCheckTimes++;
            ret = RET_TEST_MOTOR_SUCCESS;
            TRACE_CODE("电机检测1到位.\n");
        }
        else
        {
            ret = RET_TEST_MOTOR_ERROR;
            TRACE_CODE("获取电机1检测，%dms后超时！\n",TIMEOUT_TEST_PROCESS_MOTOREVENT);
        }

        if(forwardCheckTimes >= TIMES_MOTOR_CHECK)
        {
            ret = RET_TEST_MOTOR_SUCCESS;
            break;
        }

        if(times >= MAX_TIMES_MOTOR_CHECK-1)
            ret = RET_TEST_MOTOR_ERROR;
        Task_sleep((30*1000)/Clock_tickPeriod);
    }
    //电机回位----------------------------------
    uint8_t para = ACTION_TYPE_MOTOR_REVERSE;
    SendCmd_qcTest_action(&para, 1);
    //延迟300ms
    Task_sleep((300*1000)/Clock_tickPeriod);
    return ret;
}

RET_TEST_MOTOR TestProcess_motorReversal()
{
    RET_TEST_MOTOR ret;
    //检测电机反转结果
    Task_sleep((200*1000)/Clock_tickPeriod);
    uint8_t forwardCheckTimes=0;
    for(uint8_t times = 0; times < MAX_TIMES_MOTOR_CHECK; times++)
    {
        //发送电机正转动作----------------------------------
        uint8_t para = ACTION_TYPE_MOTOR_FORWARD;
        SendCmd_qcTest_action(&para, 1);
        //延时
        Task_sleep((200*1000)/Clock_tickPeriod);
        //发送电机反转动作---
        para = ACTION_TYPE_MOTOR_REVERSE;
        SendCmd_qcTest_action(&para, 1);
        //延时
        Task_sleep((200*1000)/Clock_tickPeriod);

        //等待接收返回消息
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_MOTOR_SW2, TIMEOUT_TEST_PROCESS_MOTOREVENT);
        if (events & EVENT_TESTPROCESS_MOTOR_SW2)
        {
            forwardCheckTimes++;
            ret = RET_TEST_MOTOR_SUCCESS;
            TRACE_CODE("电机检测2到位.\n");
        }
        else
        {
            ret = RET_TEST_MOTOR_ERROR;
            TRACE_CODE("获取电机2检测，%dms后超时！\n",TIMEOUT_TEST_PROCESS_MOTOREVENT);
        }

        if(forwardCheckTimes >= TIMES_MOTOR_CHECK)
        {
            ret = RET_TEST_MOTOR_SUCCESS;
            break;
        }

        if(times >= MAX_TIMES_MOTOR_CHECK-1)
            ret = RET_TEST_MOTOR_ERROR;
        Task_sleep((30*1000)/Clock_tickPeriod);
    }
    //电机回位----------------------------------
    uint8_t para = ACTION_TYPE_MOTOR_REVERSE;
    SendCmd_qcTest_action(&para, 1);
    //延迟500ms
    Task_sleep((500*1000)/Clock_tickPeriod);
    return ret;
}
