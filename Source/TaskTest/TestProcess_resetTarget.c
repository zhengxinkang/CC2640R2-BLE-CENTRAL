/*
 * TestProcess_resetTarget.c
 *
 *  Created on: 2018��9��27��
 *      Author: zheng
 */
#include "Driver_gpio.h"
#include "Driver_portUart.h"
#include "Driver_uart.h"

void TestProcess_resetTarget()
{
    //����uart������IO�������Ҹ�λĿ���
    Driver_uart_close();
    Driver_uartGpio_open();
    Driver_uartGpioSetLow();
    Driver_gpioSet(PORT_TEST_RESET_TARGET, PORT_VALUE_LOW);
    Task_sleep((10*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_RESET_TARGET, PORT_VALUE_HIGH);
    //��������3�룬�����Բ�δ���
    Task_sleep((3000*1000)/Clock_tickPeriod);
    Driver_uartGpio_close();
    Driver_uart_open();
}

void TestProcess_delay(uint32_t ms)
{
    Task_sleep((ms*1000)/Clock_tickPeriod);
}

//��λĿ�꣬�ù��ı�󣬶Ͽ���Դ�����ĵ����������
void TestProcess_resetTargetWithoutIntoTestMode()
{
    //����uart������IO�������Ҹ�λĿ���
    Driver_uart_close();
    Driver_uartGpio_open();
    Driver_uartGpioSetLow();
    Driver_gpioSet(PORT_TEST_RESET_TARGET, PORT_VALUE_LOW);
    Task_sleep((10*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_RESET_TARGET, PORT_VALUE_HIGH);
    Task_sleep((3000*1000)/Clock_tickPeriod);
    Driver_uartGpio_close();
    Driver_uart_open();
    Task_sleep((100*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_POWER_BAT, PORT_VALUE_LOW);
    Driver_gpioSet(PORT_POWER_USB, PORT_VALUE_LOW);
    Task_sleep((300*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_POWER_BAT, PORT_VALUE_HIGH);
    Driver_gpioSet(PORT_POWER_USB, PORT_VALUE_LOW);
}
