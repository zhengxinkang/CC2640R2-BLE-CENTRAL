/*
 * Driver_hc595.c
 *
 *  Created on: 2018Äê11ÔÂ13ÈÕ
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "Driver_gpio.h"

static void hc595_port_SRCLRn_clear()
{
    Driver_gpioSet(PORT_HC595_SRCLRn, PORT_VALUE_LOW);
}

static void hc595_port_SRCLRn_set()
{
    Driver_gpioSet(PORT_HC595_SRCLRn, PORT_VALUE_HIGH);
}

static void hc595_port_SRCLK_clear()
{
    Driver_gpioSet(PORT_HC595_SRCLK, PORT_VALUE_LOW);
}

static void hc595_port_SRCLK_set()
{
    Driver_gpioSet(PORT_HC595_SRCLK, PORT_VALUE_HIGH);
}

static void hc595_port_RCLK_clear()
{
    Driver_gpioSet(PORT_HC595_RCLK, PORT_VALUE_LOW);
}

static void hc595_port_RCLK_set()
{
    Driver_gpioSet(PORT_HC595_RCLK, PORT_VALUE_HIGH);
}

static void hc595_port_SER_clear()
{
    Driver_gpioSet(PORT_HC595_SER, PORT_VALUE_LOW);
}

static void hc595_port_SER_set()
{
    Driver_gpioSet(PORT_HC595_SER, PORT_VALUE_HIGH);
}

static void hc595_port_OE_clear()
{

}

//static void hc595_port_OE_set()
//{
//
//}


void Driver_hc595_init(void)
{
    hc595_port_SER_clear();
    hc595_port_SRCLK_clear();
    hc595_port_SRCLRn_clear();
    hc595_port_OE_clear();
    hc595_port_RCLK_clear();
}

void Driver_hc595_sendData(uint32_t outData)
{
    uint8_t i;

//    GPIO_PinOutSet(GPIO_HC595_SRCLRN, BIT_HC595_SRCLRN);
    hc595_port_SRCLRn_set();
    for (i = 0; i < 24; i++)
    {
        if ((outData & (0x00800000>>i) ) == (0x00800000>>i))
        {
//            LED595_DATA_SET();
            hc595_port_SER_set();
        }
        else
        {
//            LED595_DATA_RESET();
            hc595_port_SER_clear();
        }
//        OutData = OutData << 1;
//        LED595_CLK_RESET();
        hc595_port_SRCLK_clear();
//        hc595_port_RCLK_set();
//        delay_us(100);
        delay_step(1);
//        LED595_CLK_SET();
        hc595_port_SRCLK_set();
//        hc595_port_RCLK_clear();
        delay_step(1);
    }

    hc595_port_RCLK_set();
//    LED595_LATCH_RESET();
//    hc595_port_RCLK_clear();
//    delay_us(100);
    delay_step(1);
//    LED595_LATCH_SET();
//    hc595_port_RCLK_set();
    hc595_port_RCLK_clear();
}

