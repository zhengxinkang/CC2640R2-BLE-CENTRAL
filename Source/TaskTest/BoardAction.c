/*
 * BoardAction.c
 *
 *  Created on: 2018年10月13日
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "Driver_gpio.h"
#include "TaskTest.h"
#include "DataType.h"
#include "Trace.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "TestProcess_switch.h"
#include "Driver_gpio.h"
#include "Driver_uart.h"
#include "Driver_portUart.h"

void BoardAction_setModeNormal()
{
    //将uart的TX RX设置为GPIO输出高，不让目标板进入测试模式
    Driver_uart_close();
    Driver_uartGpio_open();
    Driver_uartGpioSetHigh();
    //复位目标板
    Driver_gpioSet(PORT_TEST_RESET_TARGET, PORT_VALUE_LOW);
    Task_sleep((30*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_RESET_TARGET, PORT_VALUE_HIGH);
}

void BoardAction_setting(uint32_t timeMs)
{
    //触发设置键---低电平-------------------------------
    Driver_gpioSet(PORT_TEST_SETTING_BUTTON, PORT_VALUE_HIGH);
    Task_sleep((50*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_SETTING_BUTTON, PORT_VALUE_LOW);
    Task_sleep((timeMs*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_SETTING_BUTTON, PORT_VALUE_HIGH);
    Task_sleep((50*1000)/Clock_tickPeriod);
}

void BoardAction_clear(uint32_t timeMs)
{
    //触发设置键---低电平-------------------------------
    Driver_gpioSet(PORT_TEST_CLEAR_BUTTON, PORT_VALUE_HIGH);
    Task_sleep((50*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_CLEAR_BUTTON, PORT_VALUE_LOW);
    Task_sleep((timeMs*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_CLEAR_BUTTON, PORT_VALUE_HIGH);
    Task_sleep((50*1000)/Clock_tickPeriod);
}
