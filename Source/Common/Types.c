/*
 * Types.c
 *
 *  Created on: 2018��11��24��
 *      Author: zheng
 */
#include "Types.h"

volatile static uint32_t stepCount = 0;
void delay_step(uint32_t steps) //��ʱ����
{
    for (stepCount = 0; stepCount < steps; stepCount++)
        ;
    stepCount = 0;
}

