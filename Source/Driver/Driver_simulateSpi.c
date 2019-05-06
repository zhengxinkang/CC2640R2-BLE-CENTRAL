/*
 * Driver_simulateSpi.c
 *
 *  Created on: 2018��11��14��
 *      Author: zheng
 */
#include "Driver_simulateSpi.h"
#include "Types.h"


void Write_byte_mcp3901(uchar addr, uchar dat) //��MCP3901оƬָ���ĵ�ַд������
{
    uchar i;
    addr <<= 1;
    addr &= 0xfe;

//    Adc_CS = 0;
    Driver_gpioSet(PORT_ADC_CS,       PORT_VALUE_LOW);
    for (i = 0; i < 8; i++) //д��һ�ֽڵ�ַ
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

    for (i = 0; i < 8; i++) //д��һ�ֽ�����
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
uchar Read_byte_mcp3901(uchar addr) //��MCP3901оƬ�ƶ��ĵ�ַ��������
{
    uchar dat = 0, i;
    addr <<= 1;
    addr |= 0x01;

//    while (Adc_DR == 0);
//    while (Driver_gpioGet(PORT_ADC_DR) == 0);

//    Adc_CS = 0;
    Driver_gpioSet(PORT_ADC_CS,       PORT_VALUE_LOW);
    for (i = 0; i < 8; i++) //д���ַ
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
        //todo:��������
//        Task_sleep(1);//10us
        delay_step(1);
//        Adc_SCK = 1;
        Driver_gpioSet(PORT_ADC_SCK,       PORT_VALUE_HIGH);
//        _nop_();
//        Task_sleep(1);//10us
        delay_step(1);
        addr <<= 1;
    }
    for (i = 0; i < 8; i++) //��ȡ��һ���ֽ�
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



