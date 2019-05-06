/*
 * Hal_expandInput.h
 *
 *  Created on: 2018Äê11ÔÂ23ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_HAL_HAL_EXPANDINPUT_H_
#define SOURCE_HAL_HAL_EXPANDINPUT_H_


#include "Types.h"
typedef enum{
    KEY_IN1 = 0,
    KEY_IN2,
    KEY_IN3,
    KEY_IN4,
    KEY_IN5,
    SW_IN1,
    SW_IN2,
    SW_IN3,
    R_LED4,
    R_LED3,
    R_LED2,
    R_LED1,
    MOTOR_SW2,
    MOTOR_SW1,
    NULL_1,
    NULL_2,
    R_LED12,
    R_LED11,
    R_LED10,
    R_LED9,
    R_LED8,
    R_LED7,
    R_LED6,
    R_LED5,
    INPUT_SERIAL_ERROR,
}EXPAND_INPUT_SERIAL;

//typedef enum{
//    KEY_IN1 = 0,
//    KEY_IN2,
//    KEY_IN3,
//    KEY_IN4,
//    KEY_IN5,
//    SW_IN1,
//    SW_IN2,
//    SW_IN3,
//    R_LED9,
//    R_LED10,
//    R_LED11,
//    R_LED12,
//    MOTOR_SW2,
//    MOTOR_SW1,
//    NULL_1,
//    NULL_2,
//    R_LED1,
//    R_LED2,
//    R_LED3,
//    R_LED4,
//    R_LED5,
//    R_LED6,
//    R_LED7,
//    R_LED8,
//    INPUT_SERIAL_ERROR,
//}EXPAND_INPUT_SERIAL;

PortValue Hal_expandInput_get(EXPAND_INPUT_SERIAL serial);
uint32_t Hal_expandInput_allGet();
bool IsLightOn(EXPAND_INPUT_SERIAL serial);


#endif /* SOURCE_HAL_HAL_EXPANDINPUT_H_ */
