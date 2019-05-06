/*
 * TestProcess_resetTarget.c
 *
 *  Created on: 2018年9月27日
 *      Author: zheng
 */
#include "Driver_gpio.h"
#include "Driver_portUart.h"
#include "Driver_uart.h"

void TestProcess_resetTarget()
{
    //拉低uart的两个IO，，并且复位目标板
    Driver_uart_close();
    Driver_uartGpio_open();
    Driver_uartGpioSetLow();
    Driver_gpioSet(PORT_TEST_RESET_TARGET, PORT_VALUE_LOW);
    Task_sleep((10*1000)/Clock_tickPeriod);
    Driver_gpioSet(PORT_TEST_RESET_TARGET, PORT_VALUE_HIGH);
    //不能少于3秒，否则自测未完成
    Task_sleep((3000*1000)/Clock_tickPeriod);
    Driver_uartGpio_close();
    Driver_uart_open();
}

void TestProcess_delay(uint32_t ms)
{
    Task_sleep((ms*1000)/Clock_tickPeriod);
}

//复位目标，让功耗变大，断开电源，消耗掉残余电流，
void TestProcess_resetTargetWithoutIntoTestMode()
{
    //拉低uart的两个IO，，并且复位目标板
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
