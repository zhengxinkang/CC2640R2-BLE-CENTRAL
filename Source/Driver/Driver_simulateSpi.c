/*
 * Driver_simulateSpi.c
 *
 *  Created on: 2018年11月14日
 *      Author: zheng
 */
#include "Driver_simulateSpi.h"
#include "Types.h"


void Write_byte_mcp3901(uchar addr, uchar dat) //向MCP3901芯片指定的地址写入数据
{
    uchar i;
    addr <<= 1;
    addr &= 0xfe;

//    Adc_CS = 0;
    Driver_gpioSet(PORT_ADC_CS,       PORT_VALUE_LOW);
    for (i = 0; i < 8; i++) //写入一字节地址
    {
//        Adc_SCK = 0;
        Driver_gpioSet(PORT_ADC_SCK,       PORT_VALUE_LOW);
        if (addr & 0x80)
        {
//            Adc_SDI = 1;
            Driver_gpioSet(PORT_ADC_SDI,       PORT_VALUE_HIGH);
        }
        else
        {
//            Adc_SDI = 0;
            Driver_gpioSet(PORT_ADC_SDI,       PORT_VALUE_LOW);
        }
//        _nop_();
//        Task_sleep(1);//10us
        delay_step(1);
//        Adc_SCK = 1;
        Driver_gpioSet(PORT_ADC_SCK,       PORT_VALUE_HIGH);
//        _nop_();
//        Task_sleep(1);//10us
        delay_step(1);
        addr <<= 1;
    }

    for (i = 0; i < 8; i++) //写入一字节数据
    {
//        Adc_SCK = 0;
        Driver_gpioSet(PORT_ADC_SCK,       PORT_VALUE_LOW);
        if (dat & 0x80)
        {
//            Adc_SDI = 1;
            Driver_gpioSet(PORT_ADC_SDI,       PORT_VALUE_HIGH);
        }
        else
        {
//            Adc_SDI = 0;
            Driver_gpioSet(PORT_ADC_SDI,       PORT_VALUE_LOW);
        }
//        _nop_();
//        Task_sleep(1);//10us
        delay_step(1);
//        Adc_SCK = 1;
        Driver_gpioSet(PORT_ADC_SCK,       PORT_VALUE_HIGH);
//        _nop_();
//        Task_sleep(1);//10us
        delay_step(1);
        dat <<= 1;
    }
//    Adc_SCK = 0;
    Driver_gpioSet(PORT_ADC_SCK,       PORT_VALUE_LOW);
//    Adc_CS = 1;
    Driver_gpioSet(PORT_ADC_CS,       PORT_VALUE_HIGH);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
uchar Read_byte_mcp3901(uchar addr) //从MCP3901芯片制定的地址读出数据
{
    uchar dat = 0, i;
    addr <<= 1;
    addr |= 0x01;

//    while (Adc_DR == 0);
//    while (Driver_gpioGet(PORT_ADC_DR) == 0);

//    Adc_CS = 0;
    Driver_gpioSet(PORT_ADC_CS,       PORT_VALUE_LOW);
    for (i = 0; i < 8; i++) //写入地址
    {
//        Adc_SCK = 0;
        Driver_gpioSet(PORT_ADC_SCK,       PORT_VALUE_LOW);
        if (addr & 0x80)
        {
//            Adc_SDI = 1;
            Driver_gpioSet(PORT_ADC_SDI,       PORT_VALUE_HIGH);
        }
        else
        {
//            Adc_SDI = 0;
            Driver_gpioSet(PORT_ADC_SDI,       PORT_VALUE_LOW);
        }
        //todo:错误修正
//        Task_sleep(1);//10us
        delay_step(1);
//        Adc_SCK = 1;
        Driver_gpioSet(PORT_ADC_SCK,       PORT_VALUE_HIGH);
//        _nop_();
//        Task_sleep(1);//10us
        delay_step(1);
        addr <<= 1;
    }
    for (i = 0; i < 8; i++) //读取第一个字节
    {
//        Adc_SCK = 0;
        Driver_gpioSet(PORT_ADC_SCK,       PORT_VALUE_LOW);
//        _nop_();
//        Task_sleep(1);//10us
        delay_step(1);
        dat <<= 1;
//        if (Adc_SDO == 1)
        if(Driver_gpioGet(PORT_ADC_SDO) == 1)
            dat |= 0x01;
        else
            dat &= 0xfe;
//        Adc_SCK = 1;
        Driver_gpioSet(PORT_ADC_SCK,       PORT_VALUE_HIGH);
//        _nop_();
//        Task_sleep(1);//10us
        delay_step(1);
    }
//    Adc_SCK = 0;
    Driver_gpioSet(PORT_ADC_SCK,       PORT_VALUE_LOW);
//    Adc_CS = 1;
    Driver_gpioSet(PORT_ADC_CS,       PORT_VALUE_HIGH);
    return dat;
}



