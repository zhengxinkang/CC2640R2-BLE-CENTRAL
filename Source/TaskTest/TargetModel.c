/*
 * TargetModel.c
 *
 *  Created on: 2018��9��28��
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TargetModel.h"
#include "Driver_gpio.h"
#include "Trace.h"
#define MAX_TIMES_PIN_CHECK         10
#define PASS_TIME_PIN_CHECK         8


static TARGET_MODEL targetModel = TARGET_MODEL_ERROR;
void TargetModel_init()
{
    PortValue pinValue_s31 = PORT_VALUE_HIGH;
    PortValue pinValue_s31x = PORT_VALUE_HIGH;
    uint8_t pass_times_s31 = 0;
    uint8_t pass_times_s31x = 0;
    for(uint8_t times=0; times<MAX_TIMES_PIN_CHECK; times++)
    {
        if( PORT_VALUE_LOW    ==Driver_gpioGet(PORT_TEST_MODEL_S31))
        {
            pass_times_s31++;
        }
        if( PORT_VALUE_LOW    ==Driver_gpioGet(PORT_TEST_MODEL_S31X))
        {
            pass_times_s31x++;
        }
        if(PASS_TIME_PIN_CHECK < pass_times_s31)
        {
            pinValue_s31 = PORT_VALUE_LOW;
        }
        if(PASS_TIME_PIN_CHECK < pass_times_s31x)
        {
            pinValue_s31x = PORT_VALUE_LOW;
        }
        Task_sleep((10*1000)/Clock_tickPeriod);
    }

    if
    (
        PORT_VALUE_LOW    ==pinValue_s31
        &&PORT_VALUE_HIGH   ==pinValue_s31x
    )
    {
        targetModel = TARGET_MODEL_S31;
        TRACE_DEBUG("-------------------------------------------------------|\n");
        TRACE_DEBUG("-------------------����Ŀ�꣺S31-----------------------|\n");
        TRACE_DEBUG("-------------------------------------------------------|\n");
    }
    else if
    (
        PORT_VALUE_HIGH  ==pinValue_s31
        &&PORT_VALUE_LOW   ==pinValue_s31x
    )
    {
        targetModel = TARGET_MODEL_S31X;
        TRACE_DEBUG("-------------------------------------------------------|\n");
        TRACE_DEBUG("-------------------����Ŀ�꣺S31X----------------------|\n");
        TRACE_DEBUG("-------------------------------------------------------|\n");
    }
    else
    {
        targetModel = TARGET_MODEL_ERROR;
        TRACE_DEBUG("-------------------------------------------------------|\n");
        TRACE_DEBUG("-------------------����Ŀ�꣺�����������ú�������----|\n");
        TRACE_DEBUG("-------------------------------------------------------|\n");
    }
}

TARGET_MODEL TargetModel_get()
{
    switch(targetModel)
    {
        case TARGET_MODEL_ERROR:
        {
            TRACE_DEBUG("\n");
            TRACE_DEBUG("\n\n\n-------------------------------------------------\n");
            TRACE_DEBUG("------�����ͺ����ô��������ü������ͺſ��غ�����!-----\n");
            TRACE_DEBUG("-------------------------------------------------\n\n");
            break;
        }
        case TARGET_MODEL_S31:
        {
            TRACE_DEBUG("\n\n\n-------------------------------------------------|\n");
            TRACE_DEBUG("------�����ͺ�����Ϊ��S31----------------------|\n");
            TRACE_DEBUG("-------------------------------------------------|\n\n");
            break;
        }
        case TARGET_MODEL_S31X:
        {
            TRACE_DEBUG("\n\n\n-------------------------------------------------|\n");
            TRACE_DEBUG("------�����ͺ�����Ϊ��S31X---------------------|\n");
            TRACE_DEBUG("-------------------------------------------------|\n\n");
            break;
        }
        case TARGET_MODEL_D3111:
        {
            TRACE_DEBUG("\n\n\n-------------------------------------------------|\n");
            TRACE_DEBUG("------�����ͺ�����Ϊ��D3111----------------------|\n");
            TRACE_DEBUG("-------------------------------------------------|\n\n");
            break;
        }
        case TARGET_MODEL_D3151:
        {
            TRACE_DEBUG("\n\n\n-------------------------------------------------|\n");
            TRACE_DEBUG("------�����ͺ�����Ϊ��D3151----------------------|\n");
            TRACE_DEBUG("-------------------------------------------------|\n\n");
            break;
        }
        case TARGET_MODEL_D3100:
        {
            TRACE_DEBUG("\n\n\n-------------------------------------------------|\n");
            TRACE_DEBUG("------�����ͺ�����Ϊ��D3100----------------------|\n");
            TRACE_DEBUG("-------------------------------------------------|\n\n");
            break;
        }
    }
    return targetModel;
}
