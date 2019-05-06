/*
 * Driver_simulateSpi.h
 *
 *  Created on: 2018年11月14日
 *      Author: zheng
 */

#ifndef SOURCE_DRIVER_DRIVER_SIMULATESPI_H_
#define SOURCE_DRIVER_DRIVER_SIMULATESPI_H_


#include "Driver_gpio.h"
//#include <intrins.h>
#define uchar   unsigned char
 //CHANNEL ADDRESS
#define DATA_CH0_0 0X00//通道0第1个数据寄存器地址
#define DATA_CH0_1 0X01//通道0第2个数据寄存器地址
#define DATA_CH0_2 0X02//通道0第3个数据寄存器地址

#define DATA_CH1_0 0X03//通道1第1个数据寄存器地址
#define DATA_CH1_1 0X04//通道1第2个数据寄存器地址
#define DATA_CH1_2 0X05//通道1第3个数据寄存器地址

#define MOD      0X06  //调制器输出寄存器地址
#define PHASE    0X07  //相位延迟寄存器地址
#define GAIN     0X08  //增益配置寄存器
#define STATUS   0X09  //状态/通信寄存器
#define CONFIG1  0X0A  //配置寄存器 1
#define CONFIG2  0X0B  //配置寄存器 2

//GAIN SET
#define CH0_PGA_1       0X00//CH0增益设置1，对应的输入满量程为+-500MV
#define CH0_PGA_2       0X01//CH0增益设置2，对应的输入满量程为+-250MV
#define CH0_PGA_4       0X02//CH0增益设置4，对应的输入满量程为+-125MV
#define CH0_PGA_8       0X03//CH0增益设置8，对应的输入满量程为+-62.5MV
#define CH0_PGA_16      0X04//CH0增益设置16，对应的输入满量程为+-31.25MV
#define CH0_PGA_32      0X05//CH0增益设置32，对应的输入满量程为+-15.625MV

//高速工作时电流放大比例
#define  AllCH_AmpX2    0X18//所有通道电流放大2倍
#define  CH1_AmpX2      0X10//CH1通道电流放大2倍
#define  CH0_AmpX2      0X08//CH0通道电流放大2倍
#define  NOCH_AmpX2     0X00//无通道电流放大

#define CH1_PGA_1       0X00//CH1增益设置1，对应的输入满量程为+-500MV
#define CH1_PGA_2       0X20//CH1增益设置2，对应的输入满量程为+-250MV
#define CH1_PGA_4       0X40//CH1增益设置4，对应的输入满量程为+-125MV
#define CH1_PGA_8       0X60//CH1增益设置8，对应的输入满量程为+-62.5MV
#define CH1_PGA_16      0X80//CH1增益设置16，对应的输入满量程为+-31.25MV
#define CH1_PGA_32      0Xa0//CH1增益设置32，对应的输入满量程为+-15.625MV

//STATUS REG
#define  RD_Loop1    0X00//地址计数不递增，连续读取单个寄存器
#define  RD_Loop2    0X20//寄存器类型的地址计数器循环 （默认值）

#define  Alldata_ok_on_DR    0x0c//ADC通道0和ADC通道1的数据就绪脉冲同时输出到DR引脚。
#define  Ch0data_ok_on_DR    0x04//ADC通道0的数据就绪脉冲输出到DR引脚。ADC通道1的DR信号未出现在此引脚。
#define  Ch1data_ok_on_DR    0x08//ADC通道1的数据就绪脉冲输出到DR引脚,ADC通道0的DR信号未出现在此引脚。
#define  Delaydata_ok_on_DR  0x00//两个ADC中滞后的ADC通道的数据就绪脉冲输出到DR引脚。滞后ADC的选择取决于相位寄存器
                                 //和 OSR 设置 （默认值）。
//CONFIG REG1(配置寄存器)
// 内部主时钟 （ AMCLK）预分频值
#define  Prescale0   0x00 // AMCLK = MCLK
#define  Prescale2   0x40 // AMCLK = MCLK/2
#define  Prescale4   0x80 // AMCLK = MCLK/4
#define  Prescale8   0xc0 // AMCLK = MCLK/8

//过采样率
#define  Osr32       0x00
#define  Osr64       0x10//default
#define  Osr128      0x20
#define  Osr256      0x30

#define  All_24bit             0x0c//所有通道设置为24位模式
#define  All_16bit             0x00//所有通道设置为16位模式
#define  Ch0_16bit_Ch1_24bit   0x08//CH0设置16位，CH1设置24位
#define  Ch1_16bit_Ch0_24bit   0x04//CH1设置16位，CH0设置24位

#define  Md_out                0x00 //无调制器输出被使能（默认值）

//CONFIG REG2
#define  No_shutdown      0x00//无关断模式
#define  Ch0_shutdown     0x10//CH0关断模式
#define  Ch1_shutdown     0x20//CH1关断模式
#define  All_shutdown     0x30//全部设置关断模式

#define  No_dither         0x00//无抖动控制
#define  Ch0_dither        0x04//CH0抖动控制
#define  Ch1_dither        0x08//CH1抖动控制
#define  All_dither        0x0c//全部设置抖动控制

#define  Vref_in           0x00//使用内部参考电压
#define  Vref_out          0x02//使用外部参考电压

#define  Clk_out           0x01//使用外部时钟信号
#define  Clk_xt            0x00//使用晶振时钟信号

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//sbit Adc_DR=P2^0;//定义数据就绪引脚
//sbit Adc_CS=P2^1; //片选引脚
//sbit Adc_SCK=P2^2;//时钟引脚
//sbit Adc_SDO=P2^3;//串行接口数据输出引脚
//sbit Adc_SDI=P2^4;//串行接口数据输入引脚
//sbit Adc_RST=P2^5;//定义芯片复位引脚

//sbit Led1=P0^0;//定义一个LED,用于工作指示
//sbit Led2=P0^1;//定义一个LED,用于工作指示
//sbit Led3=P0^2;//定义一个LED,用于工作指示
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//SPI MODE 0,0
void Write_byte_mcp3901(uchar addr, uchar dat); //向MCP3901芯片指定的地址写入数据
uchar Read_byte_mcp3901(uchar addr); //从MCP3901芯片制定的地址读出数据


#endif /* SOURCE_DRIVER_DRIVER_SIMULATESPI_H_ */
