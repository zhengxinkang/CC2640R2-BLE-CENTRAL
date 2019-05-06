/*
 * Hal_led.h
 *
 *  Created on: 2018Äê11ÔÂ26ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_HAL_HAL_LED_H_
#define SOURCE_HAL_HAL_LED_H_


typedef enum{
    LED_STATUS_IDLE    =0,
    LED_STATUS_BUSY    =1
}LedStatus;

void Hal_led_GYR(uint8_t green, uint8_t yellow, uint8_t red);
void Led_init();
void Led_action(uint16_t onTime, uint16_t offTime, uint16_t times);


#endif /* SOURCE_HAL_HAL_LED_H_ */
