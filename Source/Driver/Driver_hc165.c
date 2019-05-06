/*
 * Driver_hc165.c
 *
 *  Created on: 2018Äê11ÔÂ14ÈÕ
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "Driver_gpio.h"
#include "Types.h"


//#define PORT_HC165_SL                           IOID_14
//#define PORT_HC165_CLK                          IOID_15
//#define PORT_HC165_QH                           IOID_16
//
//
//
//#define HC165_CLK GPIO_Pin_8
#define HC165_DI Driver_gpioGet(PORT_HC165_QH)
//#define HC165_SL GPIO_Pin_11


uint32_t Driver_hc165_get(void)
{
    uint8_t i;

    uint32_t Hc165_temp;
    uint32_t Hc165_Value;

//    GPIO_ResetBits(GPIOA, HC165_SL);
//    GPIO_ResetBits(GPIOA, HC165_CLK);

    Driver_gpioSet(PORT_HC165_SL,       PORT_VALUE_LOW);
    Driver_gpioSet(PORT_HC165_CLK,      PORT_VALUE_LOW);

//    delay_us(1);
    delay_step(5);

//    GPIO_SetBits(GPIOA, HC165_SL);
    Driver_gpioSet(PORT_HC165_SL,       PORT_VALUE_HIGH);

    Hc165_Value = 0;

    for (i = 0; i < 24; i++)
    {
        Hc165_temp = HC165_DI;
        Hc165_Value <<= 1;
        if (Hc165_temp == 1)
        {
            Hc165_Value |= 0x0001;
        }
//        GPIO_ResetBits(GPIOA, HC165_CLK);
//        delay_us(1);
//        GPIO_SetBits(GPIOA, HC165_CLK);
//        delay_us(1);
        Driver_gpioSet(PORT_HC165_CLK,      PORT_VALUE_LOW);
        delay_step(5);
        Driver_gpioSet(PORT_HC165_CLK,      PORT_VALUE_HIGH);
        delay_step(5);
    }
//    GPIO_ResetBits(GPIOA, HC165_SL);
//    GPIO_ResetBits(GPIOA, HC165_CLK);
//    delay_us(1);
    Driver_gpioSet(PORT_HC165_SL,       PORT_VALUE_LOW);
    Driver_gpioSet(PORT_HC165_CLK,      PORT_VALUE_LOW);
    delay_step(5);

//    Temph = Hc165_Value;
//    Templ = Hc165_Value >> 8;
//    Hc165_Value = 0;
//    Hc165_Value = Temph;
//    Hc165_Value = (Hc165_Value << 8) | Templ;
    return (Hc165_Value);
}



