/*
 * BuzzPort.c
 *
 *  Created on: 2017年8月23日
 *      Author: blue.zhengxinkang
 */
/* For usleep() */
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/PWM.h>

/* Example/Board Header files */
#include "Board.h"
#include "Trace.h"

#define usInSec     1000000

volatile static uint32_t   pwmPeriod;          //周期：单位us
volatile static uint16_t   duty;               //占空比：单位us
volatile static uint8_t    pwmInitState = 0;   //初始化状态：0-未初始化，1-已初始化
volatile static PWM_Handle pwmBuzz = NULL;


void BuzzPort_init(uint32_t frequency, uint8_t dutyPercent)
{
    pwmPeriod   = usInSec/frequency;
    duty        = pwmPeriod/100 * dutyPercent;

    /* Call driver init functions. */
    PWM_init();
    PWM_Params params;
    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = duty;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;
    if(pwmBuzz)
    {
        TRACE_ERROR("pwmBuzz is inited, close old pwmBuzz.\n");
        PWM_close(pwmBuzz);
    }
    pwmBuzz = PWM_open(Board_PWM0, &params);
    if (pwmBuzz == NULL)
    {
        /* Board_PWM0 did not open */
        TRACE_ERROR("ERROR! pwm Buzz open fail!\n");
    }
    else
    {
        pwmInitState = 1;
    }
}

void BuzzPort_on()
{
    if(!pwmInitState || NULL==pwmBuzz)
    {
        TRACE_ERROR("ERROR! BuzzPort_on fail! pwm Buzz is not init!\n");
    }
    else
    {
        PWM_start(pwmBuzz);
    }
}

void BuzzPort_off()
{
    if(!pwmInitState || NULL==pwmBuzz)
    {
        TRACE_ERROR("ERROR! BuzzPort_off fail! pwm Buzz is not init!\n");
    }
    else
    {
        PWM_stop(pwmBuzz);
    }
}

//有时候需要连续调用两次，否则修改占空比可能不生效。
void BuzzPort_change(uint32_t frequency, uint8_t dutyPercent)
{
    pwmPeriod   = usInSec/frequency;
    duty        = pwmPeriod/100 * dutyPercent;

    if(!pwmInitState || NULL==pwmBuzz)
    {
        TRACE_ERROR("ERROR! BuzzPort_change fail! pwm Buzz is not init!\n");
    }
    else
    {
        PWM_setDuty(pwmBuzz, duty);

        PWM_setPeriod(pwmBuzz, pwmPeriod);
    }
}
