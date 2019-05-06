/*
 * TestProcess_switch.c
 *
 *  Created on: 2018��9��13��
 *      Author: zheng
 */
#include "TestProcess_switch.h"
#include "Driver_gpio.h"

RET_TEST_SWITCH TestProcess_switchAntiLock()
{
    RET_TEST_SWITCH ret;
    //�ı俪��״̬
    Driver_gpioSet(PORT_ANTI_LOCK, PORT_VALUE_LOW);
    //��ʱ20ms
    Task_sleep((20*1000)/Clock_tickPeriod);
    //��ѯ����״̬
    ///TBD:
}

RET_TEST_SWITCH TestProcess_switchPreventDismantle()
{
    RET_TEST_SWITCH ret;
    //�ı俪��״̬
    Driver_gpioSet(PORT_PREVENT_DISMANTLE, PORT_VALUE_LOW);
    //��ʱ20ms
    Task_sleep((20*1000)/Clock_tickPeriod);
    //��ѯ����״̬
    ///TBD:
}
