/*
 * TestProcess_switch.c
 *
 *  Created on: 2018年9月13日
 *      Author: zheng
 */
#include "TestProcess_switch.h"
#include "Driver_gpio.h"

RET_TEST_SWITCH TestProcess_switchAntiLock()
{
    RET_TEST_SWITCH ret;
    //改变开关状态
    Driver_gpioSet(PORT_ANTI_LOCK, PORT_VALUE_LOW);
    //延时20ms
    Task_sleep((20*1000)/Clock_tickPeriod);
    //查询开关状态
    ///TBD:
}

RET_TEST_SWITCH TestProcess_switchPreventDismantle()
{
    RET_TEST_SWITCH ret;
    //改变开关状态
    Driver_gpioSet(PORT_PREVENT_DISMANTLE, PORT_VALUE_LOW);
    //延时20ms
    Task_sleep((20*1000)/Clock_tickPeriod);
    //查询开关状态
    ///TBD:
}
