/*
 * Driver_ADCmpc3901.c
 *
 *  Created on: 2018年11月14日
 *      Author: zheng
 */

/*******************************************************************
 Microchip 16-24bit ADC芯片读取程序
 此程序系江南徽源电子淘宝店根据MCP3901芯片DATASHEET编写
 此程序适用于本店16位/24位ADC模块MCP3901
 2016,1,26，技术交流QQ:704002132
 http://shop57051691.taobao.com
 引用这个程序请保留这段文字说明
 *******************************************************************/
//#include <stc12c5a60s2.h>
//#include <intrins.h>
#include "Driver_ADCmpc3901.h"
#include "Driver_simulateSpi.h"
#include "Trace.h"
#include "Driver_gpio.h"
#define  uchar unsigned char
#define  uint unsigned int
//#include "mcp3901.h"
//#include "Serial.c"
uchar Adc_dat[3];
/*******************************************************************
 程序功能：延时函数
 参数：无
 ********************************************************************/
void delay(uint xx) //延时函数
{
    uint a, b;
    for (a = 0; a < 20; a++)
        for (b = xx; b > 0; b--)
            ;
}
/********************************************************************
 程序功能：初始化MCP3901.
 参数：Pga_sel0,Pga_sel1
 参数功能：PGA设置
 增益设置1，对应的输入满量程为+-500MV
 增益设置2，对应的输入满量程为+-250MV
 增益设置4，对应的输入满量程为+-125MV
 增益设置8，对应的输入满量程为+-62.5MV
 增益设置16，对应的输入满量程为+-31.25MV
 增益设置32，对应的输入满量程为+-15.625MV
 *********************************************************************/
void Init_mcp3901(uchar Pga_sel0, uchar Pga_sel1) //初始化MCP3901
{
    uchar PGAch0_SET, PGAch1_SET;
//    Adc_RST = 0;
    Driver_gpioSet(PORT_ADC_RST,       PORT_VALUE_LOW);
    delay(200);
//    Adc_RST = 1; //Reset MCP3901
    Driver_gpioSet(PORT_ADC_RST,       PORT_VALUE_HIGH);

    Write_byte_mcp3901(CONFIG2, All_shutdown); //配置CONFIG2寄存器关断模式
    Write_byte_mcp3901(CONFIG1, Prescale0 | Osr256 | All_24bit); //配置CONFIG1寄存器-无预分频，过采样率64，两个通道都为16位模式

    switch (Pga_sel0)
    //选择通道0的PGA（电压放大倍率）
    {
    case 1:
        PGAch0_SET = CH0_PGA_1;
        break;
    case 2:
        PGAch0_SET = CH0_PGA_2;
        break;
    case 4:
        PGAch0_SET = CH0_PGA_4;
        break;
    case 8:
        PGAch0_SET = CH0_PGA_8;
        break;
    case 16:
        PGAch0_SET = CH0_PGA_16;
        break;
    case 32:
        PGAch0_SET = CH0_PGA_32;
        break;
    }

    switch (Pga_sel1)
    //选择通道1的PGA（电压放大倍率）
    {
    case 1:
        PGAch1_SET = CH1_PGA_1;
        break;
    case 2:
        PGAch1_SET = CH1_PGA_2;
        break;
    case 4:
        PGAch1_SET = CH1_PGA_4;
        break;
    case 8:
        PGAch1_SET = CH1_PGA_8;
        break;
    case 16:
        PGAch1_SET = CH1_PGA_16;
        break;
    case 32:
        PGAch1_SET = CH1_PGA_32;
        break;
    }
    Write_byte_mcp3901(GAIN, PGAch0_SET | PGAch1_SET | AllCH_AmpX2); //设置电压增益，电流增益（放大电流2倍，使用内部CLOCK，则不要放大）?
    Write_byte_mcp3901(STATUS, Alldata_ok_on_DR | RD_Loop1); //配置状态寄存器，两通道数据OK时在DR引脚输出脉冲，
                                                             // 地址计数不递增，连续读取单个寄存器
    Write_byte_mcp3901(CONFIG2, No_shutdown | All_dither | Vref_out | Clk_xt); //无关断，无消除抖动，
                                                                               //外部参考电压,晶振作为时钟
}

/***********************************************************************
 程序功能：读取一个通道的ADC数值。
 参数：ch
 参数功能：选择ADC通道
 参数值：0---ADC通道0
 1---ADC通道1
 ***********************************************************************/
double Read_1CH_adc(uchar ch)
{
    double adda1;
//    uchar ds[10];
//    SendByte('A');
//    SendByte(ch | 0x30);
//    SendByte(':');

    if (ch == 0)                                                         //读取通道0
    {
        Adc_dat[0] = Read_byte_mcp3901(DATA_CH0_0);                    //读取3字节数据
        Adc_dat[1] = Read_byte_mcp3901(DATA_CH0_1);
        Adc_dat[2] = Read_byte_mcp3901(DATA_CH0_2);
    }
    else //读取通道1
    {
        Adc_dat[0] = Read_byte_mcp3901(DATA_CH1_0);
        Adc_dat[1] = Read_byte_mcp3901(DATA_CH1_1);
        Adc_dat[2] = Read_byte_mcp3901(DATA_CH1_2);
    }

    if (Adc_dat[0] & 0x80) //判断电压极性
    {
        Adc_dat[0] = ~Adc_dat[0];
        Adc_dat[1] = ~Adc_dat[1];
        Adc_dat[2] = ~Adc_dat[2];
//        SendByte('-');
    }
    else
//        SendByte('+');
    adda1 = (Adc_dat[0] * 65536 + Adc_dat[1] * 256 + Adc_dat[2]);
    adda1 = adda1 * 0.0993; //默认PGA为1，即满量程+-500MV
                            //5000000/((0.5/2.5)*8388608*PGA*3)=0.0993(外部参考电压)
                            //5000000/((0.5/2.37)*8388608*PGA*3)=0.0941（内部参考电压）
    //adda1=(adda1*23.7)/PGA;//若PGA大于1，则在计算的时候需要除以PGA的倍率值，
    //才能还原实际采集的电压值
//    ds[0] = (long) adda1 / 100000 | 0x30;
//    ds[1] = (long) adda1 % 100000 / 10000 | 0x30;
//    ds[2] = (long) adda1 % 10000 / 1000 | 0x30;
//    ds[3] = '.';
//    ds[4] = (long) adda1 % 1000 / 100 | 0x30;
//    ds[5] = (long) adda1 % 100 / 10 | 0x30;
//    ds[6] = '\r';
//    ds[7] = '\n';
//    ds[8] = '\0';
//    Printf(ds);                           //串口输出数据
//    TRACE_DEBUG("%s",ds);
//    delay(7200);                           //读取程序间隔延时，减小此数值可提高采样率
    adda1 = adda1/GAIN_NUM_ADC - BIAS_VOLTAGE;
    if(adda1 < 0)
        adda1 = 0;
//    TRACE_CODE("%lf uV\r\n",adda1);
    return (adda1);
}
/***************************************************************************
 程序功能：主函数，MCP3901芯片读取ADC值。
 参数：无
 ****************************************************************************/
                           //在初始化MCP3901里设置两个通道的PGA


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

