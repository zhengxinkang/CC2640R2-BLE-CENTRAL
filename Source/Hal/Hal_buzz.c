/*
 * Buzz.c
 *
 *  Created on: 2017Äê8ÔÂ25ÈÕ
 *      Author: blue.zhengxinkang
 */
#include <Driver_buzzPort.h>
#include <Hal_buzz.h>
#include "util.h"
#include "Trace.h"

#define BUZZ_TIMER_EVT_PERIOD           10
static Clock_Struct buzzTimerClock;

volatile uint16_t buzz_step = 0;
volatile uint16_t buzz_period = BUZZ_TIMER_EVT_PERIOD;
volatile uint16_t buzz_onTime = 0;
volatile uint16_t buzz_offTime = 0;
volatile BuzzStatus buzzStatus = BUZZ_STATUS_IDLE;

void buzzTimer_handle(UArg arg);

void Buzz_init()
{
    BuzzPort_init(2731, 20);

    Util_constructClock(&buzzTimerClock, buzzTimer_handle,
                        BUZZ_TIMER_EVT_PERIOD, 0, false, 0);
    buzzStatus = BUZZ_STATUS_IDLE;
}

void Buzz_action(uint16_t onTime, uint16_t offTime, uint16_t times)
{
    buzz_onTime = onTime;
    buzz_offTime = offTime;
    buzz_step = times*2;

    buzzStatus = BUZZ_STATUS_BUSY;
    BuzzPort_on();
    Util_restartClock(&buzzTimerClock, buzz_onTime);
}

uint8_t Buzz_isBusy()
{
    uint8_t ret;
    if(BUZZ_STATUS_BUSY == buzzStatus)
        ret = true;
    else if(BUZZ_STATUS_IDLE == buzzStatus)
        ret = false;
    return ret;
}

void buzzTimer_handle(UArg arg)
{
    if(--buzz_step > 0)
    {
        if(buzz_step%2)
        {
            BuzzPort_off();
            Util_restartClock(&buzzTimerClock, buzz_offTime);
        }
        else
        {
            BuzzPort_on();
            Util_restartClock(&buzzTimerClock, buzz_onTime);
        }
    }
    else
    {
        BuzzPort_off();
        Util_stopClock(&buzzTimerClock);
        buzzStatus = BUZZ_STATUS_IDLE;
    }
}
