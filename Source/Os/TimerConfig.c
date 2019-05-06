/*
 * TimerConfig.c
 *
 *  Created on: 2018年8月15日
 *      Author: zheng
 */
#include "TimerConfig.h"

// Clock instances for internal periodic events.
Clock_Struct sendDataClock;

extern void Timer_sendDataHandle(UArg arg);


static void TimerConfig_create(Clock_Struct *pClock, Clock_FuncPtr clockCB, uint32_t clockDuration)
{
    Util_constructClock(pClock, clockCB, clockDuration, 0, false, 0);
}

extern void TimerConfig_init()
{
    TimerConfig_create(&sendDataClock, Timer_sendDataHandle, TIMER_SENDDATA_PERIOD_DEFAULT);
}

extern void TimerConfig_start(Clock_Struct *pClock, uint32_t timeout)
{
    Util_restartClock(pClock, timeout);
}

extern void TimerConfig_stop(Clock_Struct *pClock)
{
    Util_stopClock(pClock);
}
