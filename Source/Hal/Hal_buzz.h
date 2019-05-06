/*
 * Buzz.h
 *
 *  Created on: 2017Äê8ÔÂ25ÈÕ
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_UI_BUZZ_H_
#define SOURCE_UI_BUZZ_H_


#include "stdint.h"

typedef enum{
    BUZZ_STATUS_IDLE    =0,
    BUZZ_STATUS_BUSY    =1
}BuzzStatus;

void Buzz_init();
void Buzz_action(uint16_t onTime, uint16_t offTime, uint16_t times);
uint8_t Buzz_isBusy();


#endif /* SOURCE_UI_BUZZ_H_ */
