/*
 * TestProcess_motor.c
 *
 *  Created on: 2018年9月12日
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "TestProcess_motor.h"
#include "Driver_gpio.h"
#include "DataType.h"

#define MAX_TIMES_MOTOR_CHECK   6
#define TIMES_MOTOR_CHECK       3

RET_TEST_MOTOR TestProcess_motorForward()
{
    RET_TEST_MOTOR ret;
    //发送电机正转动作----------------------------------
    uint8_t para = ACTION_TYPE_MOTOR_FORWARD;
    SendCmd_qcTest_action(&para, 1);
    //延迟500ms
    Task_sleep((500*1000)/Clock_tickPeriod);
    //检测电机正转结果
    uint8_t forwardCheckTimes=0;
    for(uint8_t times = 0; times < MAX_TIMES_MOTOR_CHECK; times++)
    {
        PortValue portValue=Driver_gpioGet(PORT_MOTOR_FORWARD);
        if(0 == portValue)
            forwardCheckTimes++;
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
    para = ACTION_TYPE_MOTOR_REVERSE;
    SendCmd_qcTest_action(&para, 1);
    //延迟500ms
    Task_sleep((500*1000)/Clock_tickPeriod);
    return ret;
}

RET_TEST_MOTOR TestProcess_motorReversal()
{
    RET_TEST_MOTOR ret;
    //发送电机正转动作----------------------------------
    uint8_t para = ACTION_TYPE_MOTOR_FORWARD;
    SendCmd_qcTest_action(&para, 1);
    //延迟500ms
    Task_sleep((500*1000)/Clock_tickPeriod);
    //发送电机反转动作----------------------------------
    para = ACTION_TYPE_MOTOR_REVERSE;
    SendCmd_qcTest_action(&para, 1);

    //延迟500ms
    Task_sleep((500*1000)/Clock_tickPeriod);
    //检测电机反转结果
    uint8_t reversalCheckTimes=0;
    for(uint8_t times = 0; times < MAX_TIMES_MOTOR_CHECK; times++)
    {
        PortValue portValue=Driver_gpioGet(PORT_MOTOR_REVERSAL);
        if(0 == portValue)
            reversalCheckTimes++;
        if(reversalCheckTimes >= TIMES_MOTOR_CHECK)
        {
            ret = RET_TEST_MOTOR_SUCCESS;
            break;
        }
        if(times >= MAX_TIMES_MOTOR_CHECK-1)
            ret = RET_TEST_MOTOR_ERROR;
        Task_sleep((30*1000)/Clock_tickPeriod);
    }
    return ret;
}
