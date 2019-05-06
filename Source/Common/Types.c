/*
 * Types.c
 *
 *  Created on: 2018年11月24日
 *      Author: zheng
 */
#include "Types.h"

volatile static uint32_t stepCount = 0;
void delay_step(uint32_t steps) //延时函数
{
    for (stepCount = 0; stepCount < steps; stepCount++)
        ;
    stepCount = 0;
}

