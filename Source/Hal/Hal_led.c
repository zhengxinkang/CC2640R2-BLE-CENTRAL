/*
 * Hal_led.c
 *
 *  Created on: 2018年11月26日
 *      Author: zheng
 */
#include <stdbool.h>
#include  "Hal_expandOutput.h"

void Hal_led_GYR(uint8_t green, uint8_t yellow, uint8_t red)
{
    bool s_green, s_yellow, s_red;

    if(green != 0xff)
    {
        Hal_expandOutput(LED_G, (PortValue)(!green));
    }
    if(yellow != 0xff)
    {
        Hal_expandOutput(LED_Y, (PortValue)(!yellow));
    }
    if(red != 0xff)
    {
        Hal_expandOutput(LED_R, (PortValue)(!red));
    }
}

/*
 * Buzz.c
 *
 *  Created on: 2017年8月25日
 *      Author: blue.zhengxinkang
 */

#include <Hal_led.h>
#include "util.h"
#include "Trace.h"

#define LED_TIMER_EVT_PERIOD           10
static Clock_Struct ledTimerClock;

volatile uint16_t led_step = 0;
volatile uint16_t led_period = LED_TIMER_EVT_PERIOD;
volatile uint16_t led_onTime = 0;
volatile uint16_t led_offTime = 0;
volatile LedStatus ledStatus = LED_STATUS_IDLE;

void ledTimer_handle(UArg arg);

void Led_init()
{
    Util_constructClock(&ledTimerClock, ledTimer_handle,
                        LED_TIMER_EVT_PERIOD, 0, false, 0);
    ledStatus = LED_STATUS_IDLE;
}

void Led_action(uint16_t onTime, uint16_t offTime, uint16_t times)
{
    led_onTime = onTime;
    led_offTime = offTime;
    led_step = times*2;

    ledStatus = LED_STATUS_BUSY;
    Hal_led_GYR(0xff, 1, 0xff);
//    BuzzPort_on();
    Util_restartClock(&ledTimerClock, led_onTime);
}

uint8_t Led_isBusy()
{
    uint8_t ret;
    if(LED_STATUS_BUSY == ledStatus)
        ret = true;
    else if(LED_STATUS_IDLE == ledStatus)
        ret = false;
    return ret;
}

void ledTimer_handle(UArg arg)
{
    if(--led_step > 0)
    {
        if(led_step%2)
        {
            Hal_led_GYR(0xff, 0, 0xff);
//            BuzzPort_off();
            Util_restartClock(&ledTimerClock, led_offTime);
        }
        else
        {
            Hal_led_GYR(0xff, 1, 0xff);
//            LedPort_on();
            Util_restartClock(&ledTimerClock, led_onTime);
        }
    }
    else
    {
        Hal_led_GYR(0xff, 0, 0xff);
//        LedPort_off();
        Util_stopClock(&ledTimerClock);
        ledStatus = LED_STATUS_IDLE;
    }
}
