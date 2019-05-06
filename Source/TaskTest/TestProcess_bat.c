/*
 * TestProcess_bat.c
 *
 *  Created on: 2018Äê9ÔÂ13ÈÕ
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TestProcess_bat.h"
#include "Driver_adc.h"

#define MAX_TIMES_BAT_CHECK     10
#define TIMES_BAT_CHECK         7

RET_TEST_BAT TestProcess_bat()
{
    RET_TEST_BAT ret;
    uint8_t checkTimes = 0;
    uint8_t checkFailTimes = 0;
    for(uint8_t times = 0; times < MAX_TIMES_BAT_CHECK; times++)
    {
        uint32_t adcValue = Driver_adcGet();
        if(adcValue > 3200000&&adcValue < 3400000)
            checkTimes++;
        else
            checkFailTimes++;

        if(checkTimes >= TIMES_BAT_CHECK)
            ret = RET_TEST_BAT_SUCCESS;
            break;

        if(times >= MAX_TIMES_BAT_CHECK)
            ret = RET_TEST_BAT_ERROR;
        Task_sleep((30*1000)/Clock_tickPeriod);
    }
}


