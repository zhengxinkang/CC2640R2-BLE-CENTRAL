/*
 * Driver_gpio.h
 *
 *  Created on: 2018Äê9ÔÂ12ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_DRIVER_DRIVER_GPIO_H_
#define SOURCE_DRIVER_DRIVER_GPIO_H_


#include "Types.h"
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/PIN.h>
#include "Board.h"

uint8_t     Driver_gpioInit();
uint8_t     Driver_gpioSet(PIN_Id pinId, PortValue portValue);
PortValue   Driver_gpioGet(PIN_Id pinId);


#endif /* SOURCE_DRIVER_DRIVER_GPIO_H_ */
