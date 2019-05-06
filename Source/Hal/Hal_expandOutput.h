/*
 * Hal_expandOutput.h
 *
 *  Created on: 2018Äê11ÔÂ23ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_HAL_HAL_EXPANDOUTPUT_H_
#define SOURCE_HAL_HAL_EXPANDOUTPUT_H_


#include "Types.h"

typedef enum{
    KEY_12 = 0,
    KEY_11,
    KEY_10,
    KEY_9,
    KEY_8,
    KEY_7,
    KEY_6,
    KEY_5,
    KEY_4,
    KEY_3,
    KEY_2,
    KEY_1,
    CLMA,
    CLMB,
    PWR_SW_YJ,
    PWR_SW_BAT,
    LATCH_BOLT,
    USR_SET,
    USR_CLR,
    USR_RELOCK,
    FQ_SW,
    LED_G,
    LED_Y,
    LED_R,
    SERIAL_ERROR,
}EXPAND_OUTPUT_SERIAL;

//typedef enum{
//    KEY_1 = 0,
//    KEY_2,
//    KEY_3,
//    KEY_4,
//    KEY_5,
//    KEY_6,
//    KEY_7,
//    KEY_8,
//    KEY_9,
//    KEY_10,
//    KEY_11,
//    KEY_12,
//    CLMA,
//    CLMB,
//    PWR_SW_YJ,
//    PWR_SW_BAT,
//    LATCH_BOLT,
//    USR_SET,
//    USR_CLR,
//    USR_RELOCK,
//    FQ_SW,
//    LED_Y,
//    LED_G,
//    LED_R,
//    SERIAL_ERROR,
//}EXPAND_OUTPUT_SERIAL;

void Hal_expandOutput_init();
void Hal_expandOutput(EXPAND_OUTPUT_SERIAL serial, PortValue value);


#endif /* SOURCE_HAL_HAL_EXPANDOUTPUT_H_ */
