/*
 * BoardAction.c
 *
 *  Created on: 2018��10��13��
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
    //��uart��TX RX����ΪGPIO����ߣ�����Ŀ���������ģʽ
    Driver_uart_close();
    Driver_uartGpio_open();
    Driver_uartGpioSetHigh();
    //��λĿ���
    Driver_gpioSet(PORT_TEST_RESET_TARGET, PORT_VALUE_LOW);
    Task_sleep((30*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_RESET_TARGET, PORT_VALUE_HIGH);
}

void BoardAction_setting(uint32_t timeMs)
{
    //�������ü�---�͵�ƽ-------------------------------
    Driver_gpioSet(PORT_TEST_SETTING_BUTTON, PORT_VALUE_HIGH);
    Task_sleep((50*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_SETTING_BUTTON, PORT_VALUE_LOW);
    Task_sleep((timeMs*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_SETTING_BUTTON, PORT_VALUE_HIGH);
    Task_sleep((50*1000)/Clock_tickPeriod);
}

void BoardAction_clear(uint32_t timeMs)
{
    //�������ü�---�͵�ƽ-------------------------------
    Driver_gpioSet(PORT_TEST_CLEAR_BUTTON, PORT_VALUE_HIGH);
    Task_sleep((50*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_CLEAR_BUTTON, PORT_VALUE_LOW);
    Task_sleep((timeMs*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_CLEAR_BUTTON, PORT_VALUE_HIGH);
    Task_sleep((50*1000)/Clock_tickPeriod);
}
