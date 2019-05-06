/*
 * Hal_led.c
 *
 *  Created on: 2018Äê11ÔÂ26ÈÕ
 *      Author: zheng
 */
#include <stdbool.h>
#include  "Hal_expandOutput.h"

void Hal_led_YGR(bool yellow, bool green, bool red)
{
    Hal_expandOutput(LED_Y, (PortValue)(!yellow));
    Hal_expandOutput(LED_G, (PortValue)(!green));
    Hal_expandOutput(LED_R, (PortValue)(!red));
}

