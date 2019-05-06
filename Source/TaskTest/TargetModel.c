/*
 * TargetModel.c
 *
 *  Created on: 2018年9月28日
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
static void Trace_model(TARGET_MODEL model);

void TargetModel_init(TARGET_MODEL model)
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

    //如果初始化设置了类型，则无需去获取
    if(model != TARGET_MODEL_UNKNOW)
    {
        targetModel = model;
    }
    else if(PORT_VALUE_LOW==pinValue_s31 && PORT_VALUE_HIGH==pinValue_s31x)
    {
        targetModel = TARGET_MODEL_S31;
    }
    else if(PORT_VALUE_HIGH  ==pinValue_s31 && PORT_VALUE_LOW   ==pinValue_s31x)
    {
        targetModel = TARGET_MODEL_S31X;
    }
    else
    {
        targetModel = TARGET_MODEL_ERROR;
    }
    Trace_model(targetModel);
}

TARGET_MODEL TargetModel_get()
{
    Trace_model(targetModel);
    return targetModel;
}

static void Trace_model(TARGET_MODEL model)
{
    switch(model)
    {
        case TARGET_MODEL_UNKNOW:
        case TARGET_MODEL_ERROR:
        {
            TRACE_DEBUG("-------------------------------------------------------|\n");
            TRACE_DEBUG("-------------------测试目标：错误，请修改配置后重启！----|\n");
            TRACE_DEBUG("-------------------------------------------------------|\n");
            break;
        }
        case TARGET_MODEL_S31:
        {
            TRACE_DEBUG("-------------------------------------------------------|\n");
            TRACE_DEBUG("-------------------测试目标：S31------------------------|\n");
            TRACE_DEBUG("-------------------------------------------------------|\n\n");
            break;
        }
        case TARGET_MODEL_S31X:
        {
            TRACE_DEBUG("-------------------------------------------------------|\n");
            TRACE_DEBUG("-------------------测试目标：S31X-----------------------|\n");
            TRACE_DEBUG("-------------------------------------------------------|\n\n");
            break;
        }
        case TARGET_MODEL_D3111:
        {
            TRACE_DEBUG("-------------------------------------------------------|\n");
            TRACE_DEBUG("-------------------测试目标：D3111----------------------|\n");
            TRACE_DEBUG("-------------------------------------------------------|\n\n");
            break;
        }
        case TARGET_MODEL_D3151:
        {
            TRACE_DEBUG("-------------------------------------------------------|\n");
            TRACE_DEBUG("-------------------测试目标：D3151----------------------|\n");
            TRACE_DEBUG("-------------------------------------------------------|\n\n");
            break;
        }
        case TARGET_MODEL_D3100:
        {
            TRACE_DEBUG("-------------------------------------------------------|\n");
            TRACE_DEBUG("-------------------测试目标：D3100----------------------|\n");
            TRACE_DEBUG("-------------------------------------------------------|\n\n");
            break;
        }
    }
}
