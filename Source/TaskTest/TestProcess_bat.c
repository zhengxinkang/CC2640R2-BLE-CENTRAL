/*
 * TestProcess_bat.c
 *
 *  Created on: 2018��9��13��
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TestProcess_bat.h"
#include "Driver_adc.h"
#include "Driver_gpio.h"
#include "Trace.h"

#define MAX_TIMES_BAT_CHECK         5
#define TIMES_BAT_CHECK             3

RET_TEST_BAT TestProcess_bat()
{
    RET_TEST_BAT ret = RET_TEST_BAT_SUCCESS;
    uint8_t checkSuccessTimes = 0;
    uint8_t checkFailTimes = 0;
    uint8_t times = 0;

    Driver_gpioSet(PORT_POWER_BAT, PORT_VALUE_LOW);
    Driver_gpioSet(PORT_POWER_USB, PORT_VALUE_LOW);
    Task_sleep((300*1000)/Clock_tickPeriod);
    //��Դѡ��-BAT
    Driver_gpioSet(PORT_POWER_BAT, PORT_VALUE_HIGH);
    Driver_gpioSet(PORT_POWER_USB, PORT_VALUE_LOW);
    Task_sleep((100*1000)/Clock_tickPeriod);
    if(RET_TEST_BAT_SUCCESS == ret)
    {
        for (times = 0; times < MAX_TIMES_BAT_CHECK; times++)
        {
            uint32_t adcValue = Driver_adcGet();
            if (adcValue > 2700000 && adcValue < 3400000)
                checkSuccessTimes++;
            else
                checkFailTimes++;

            if (checkSuccessTimes >= TIMES_BAT_CHECK)
            {
                ret = RET_TEST_BAT_SUCCESS;
                break;
            }
            if (times >= MAX_TIMES_BAT_CHECK - 1)
            {
                ret = RET_TEST_BAT_ERROR;
                break;
            }
            Task_sleep((10 * 1000) / Clock_tickPeriod);
        }
    }

    return ret;
}

RET_TEST_BAT TestProcess_usb()
{
    RET_TEST_BAT ret = RET_TEST_BAT_SUCCESS;
    uint8_t checkSuccessTimes = 0;
    uint8_t checkFailTimes = 0;
    uint8_t times = 0;

    Driver_gpioSet(PORT_POWER_BAT, PORT_VALUE_LOW);
    Driver_gpioSet(PORT_POWER_USB, PORT_VALUE_LOW);
    Task_sleep((300*1000)/Clock_tickPeriod);
    //��Դѡ��-USB
    Driver_gpioSet(PORT_POWER_BAT, PORT_VALUE_LOW);
    Driver_gpioSet(PORT_POWER_USB, PORT_VALUE_HIGH);
    Task_sleep((500*1000)/Clock_tickPeriod);
    if(RET_TEST_BAT_SUCCESS == ret)
    {
        for (times = 0; times < MAX_TIMES_BAT_CHECK; times++)
        {
            uint32_t adcValue = Driver_adcGet();
            if (adcValue > 3200000 && adcValue < 3400000)
                checkSuccessTimes++;
            else
                checkFailTimes++;

            if (checkSuccessTimes >= TIMES_BAT_CHECK)
            {
                ret = RET_TEST_BAT_SUCCESS;
                break;
            }
            if (times >= MAX_TIMES_BAT_CHECK - 1)
            {
                ret = RET_TEST_BAT_ERROR;
                break;
            }
            Task_sleep((10 * 1000) / Clock_tickPeriod);
        }
    }

    return ret;
}