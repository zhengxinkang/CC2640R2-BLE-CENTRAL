/*
 * TestProcess_resetTarget.c
 *
 *  Created on: 2018年9月27日
 *      Author: zheng
 */
#include "Driver_gpio.h"
#include "TestProcess_uartEcho.h"
#include "Driver_portUart.h"
#include "Driver_uart.h"
#include "Lock_atcion.h"
#include "Trace.h"

#define MAX_TIMES_RESETTARGET   3
void TestProcess_resetTarget()
{
    for(uint8_t time=0; time<MAX_TIMES_RESETTARGET; time++)
    {
        //拉低uart的两个IO，，并且复位目标板
        Driver_uart_close();
        Driver_uartGpio_open();
        Driver_uartGpioSetLow();
        Driver_gpioSet(PORT_TEST_RESET_TARGET, PORT_VALUE_LOW);
        Task_sleep((100*1000)/Clock_tickPeriod);
        Driver_gpioSet(PORT_TEST_RESET_TARGET, PORT_VALUE_HIGH);

        //不能少于4秒，否则自测未完成
        Task_sleep((4000*1000)/Clock_tickPeriod);
        Driver_uartGpio_close();
        Driver_uart_open();
        if(RET_TEST_UARTECHO_SUCCESS != TestProcess_uartEchoCheck())
        {
            if(time < MAX_TIMES_RESETTARGET-1)
                TRACE_ERROR("重启失败，锁主板没有进入测试模式。再次尝试。\n");
            else
                TRACE_ERROR("重启失败，锁主板没有进入测试模式。%d次尝试后失败放弃。\n", MAX_TIMES_RESETTARGET);
        }
        else
        {
            TRACE_CODE("重启成功，锁主板进入测试模式。\n");
            break;
        }
    }
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
//    Driver_gpioSet(PORT_POWER_BAT, PORT_VALUE_LOW);
//    Driver_gpioSet(PORT_POWER_USB, PORT_VALUE_LOW);
    Lock_action_power(false, false);
    Task_sleep((300*1000)/Clock_tickPeriod);
//    Driver_gpioSet(PORT_POWER_BAT, PORT_VALUE_HIGH);
//    Driver_gpioSet(PORT_POWER_USB, PORT_VALUE_LOW);
    Lock_action_power(false, true);
}
