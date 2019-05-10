/*
 * Hal_electricCurrent.h
 *
 *  Created on: 2018Äê11ÔÂ16ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_HAL_HAL_ELECTRICCURRENT_H_
#define SOURCE_HAL_HAL_ELECTRICCURRENT_H_


void Hal_electricCurrent_init();
void Hal_electricCurrent_offsetRead();
uint32_t Hal_electricCurrent_offsetGet();
int32_t avgCurrentCount(int32_t currentThis, bool isShift, uint8_t times);


#endif /* SOURCE_HAL_HAL_ELECTRICCURRENT_H_ */
