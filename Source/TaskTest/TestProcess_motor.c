/*
 * TestProcess_motor.c
 *
 *  Created on: 2018��9��12��
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TestProcess_motor.h"
#include "Driver_gpio.h"

#define MAX_TIMES_MOTOR_CHECK   5
#define TIMES_MOTOR_CHECK       3

RET_TEST_MOTOR TestProcess_motorForward()
{
    RET_TEST_MOTOR ret;
    //���͵����תָ��----------------------------------

    //�ӳ�500ms
    Task_sleep((200*1000)/Clock_tickPeriod);
    //�������ת���
    uint8_t forwardCheckTimes=0;
    for(uint8_t times = 0; times < MAX_TIMES_MOTOR_CHECK; times++)
    {
        PortValue portValue=Driver_gpioGet(PORT_MOTOR_FORWARD);
        if(0 == portValue)
            forwardCheckTimes++;
        if(forwardCheckTimes >= TIMES_MOTOR_CHECK)
            ret = RET_TEST_MOTOR_SUCCESS;
            break;
        if(times >= MAX_TIMES_MOTOR_CHECK)
            ret = RET_TEST_MOTOR_ERROR;
        Task_sleep((30*1000)/Clock_tickPeriod);
    }
    return ret;
}

RET_TEST_MOTOR TestProcess_motorReversal()
{
    RET_TEST_MOTOR ret;
    //���͵����תָ��----------------------------------

    //�ӳ�500ms
    Task_sleep((200*1000)/Clock_tickPeriod);
    //�������ת���
    uint8_t reversalCheckTimes=0;
    for(uint8_t times = 0; times < MAX_TIMES_MOTOR_CHECK; times++)
    {
        PortValue portValue=Driver_gpioGet(PORT_MOTOR_REVERSAL);
        if(0 == portValue)
            reversalCheckTimes++;
        if(reversalCheckTimes >= TIMES_MOTOR_CHECK)
            ret = RET_TEST_MOTOR_SUCCESS;
            break;
        if(times >= MAX_TIMES_MOTOR_CHECK)
            ret = RET_TEST_MOTOR_ERROR;
        Task_sleep((30*1000)/Clock_tickPeriod);
    }
    return ret;
}
