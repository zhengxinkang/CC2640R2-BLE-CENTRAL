/*
 * TestProcess_bat.c
 *
 *  Created on: 2018年9月13日
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "BF_Util.h"
#include "TestProcess_bat.h"
#include "Driver_adc.h"
#include "Driver_gpio.h"
#include "Trace.h"
#include "Lock_atcion.h"

#define MAX_TIMES_VOLTAGE_CHECK     5
#define TIMES_VOLTAGE_CHECK         3
#define VOLTAGE_MIN                 1500000
#define VOLTAGE_MAX                 1700000

RET_TEST_BAT TestProcess_bat()
{
    RET_TEST_BAT ret = RET_TEST_BAT_SUCCESS;
    uint8_t checkSuccessTimes = 0;
    uint8_t checkFailTimes = 0;
    uint8_t times = 0;

    //电源全部关闭
    Lock_action_power(false, false);
    BF_taskSleepMs(200);

    //电源选择-BAT
    Lock_action_power(true, false);
    BF_taskSleepMs(200);

    if(RET_TEST_BAT_SUCCESS == ret)
    {
        for (times = 0; times < MAX_TIMES_VOLTAGE_CHECK; times++)
        {
            uint32_t adcValue = Driver_adcGet();
            if (adcValue > VOLTAGE_MIN && adcValue < VOLTAGE_MAX)
                checkSuccessTimes++;
            else
                checkFailTimes++;

            if (checkSuccessTimes >= TIMES_VOLTAGE_CHECK)
            {
                ret = RET_TEST_BAT_SUCCESS;
                break;
            }
            if (times >= MAX_TIMES_VOLTAGE_CHECK - 1)
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

    //电源全部关闭
    Lock_action_power(false, false);
    BF_taskSleepMs(200);

    //电源选择-USB
    Lock_action_power(false, true);
    BF_taskSleepMs(200);

    if(RET_TEST_BAT_SUCCESS == ret)
    {
        for (times = 0; times < MAX_TIMES_VOLTAGE_CHECK; times++)
        {
            uint32_t adcValue = Driver_adcGet();
            if (adcValue > VOLTAGE_MIN && adcValue < VOLTAGE_MAX)
                checkSuccessTimes++;
            else
                checkFailTimes++;

            if (checkSuccessTimes >= TIMES_VOLTAGE_CHECK)
            {
                ret = RET_TEST_BAT_SUCCESS;
                break;
            }
            if (times >= MAX_TIMES_VOLTAGE_CHECK - 1)
            {
                ret = RET_TEST_BAT_ERROR;
                break;
            }
            Task_sleep((10 * 1000) / Clock_tickPeriod);
        }
    }

    return ret;
}
