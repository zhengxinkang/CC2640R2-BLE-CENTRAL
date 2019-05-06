///*
// * Driver_CS1231.c
// *
// *  Created on: 2018年11月23日
// *      Author: zheng
// */
//
//#include "Trace.h"
//#include "Driver_gpio.h"
//#include <ti/sysbios/knl/Task.h>
//#include <ti/sysbios/knl/Clock.h>
//
//#define u8 uint8_t
//#define u32 uint32_t
////-----------------------------------------------------
////函数名称： void Driver_CS1231_Clk(void)
////功 能：时钟脉冲
////说 明：
////-----------------------------------------------------
//static void Driver_CS1231_Clk(void)
//{
////    IO_ADC_SCLK = HIGH;
////    Delay();
////    IO_ADC_SCLK = LOW;
////    Delay();
//
//    Driver_gpioSet(PORT_SPI_CLK, PORT_VALUE_HIGH);
//    Task_sleep(1);
//    Driver_gpioSet(PORT_SPI_CLK, PORT_VALUE_LOW);
//    Task_sleep(1);
//}
//
////-----------------------------------------------------
////函数名称：从 CS1231/CS1232 读取 1 字节的数据
////功 能：
////说 明：返回 1 字节的无符号字符型 rdata
////-----------------------------------------------------
//u8 Read_byte_adcIC(void)
//{
//    u8 i;
//    u8 rdata = 0; //初始化返回值为 0
//    for(i = 0; i < 8; i++)
//    {
//        rdata <<= 1; //返回值左移 1 位
////        CS1231_Clk();//上升沿接收数据,下降沿更新数据
//        Driver_CS1231_Clk();
////        if(IO_ADC_DDAT)
//        if( Driver_gpioGet(PORT_SPI_DATA) )
//        {
//            rdata += 1; //若数据线上数据为 1，则返回值加 1
//        }
//    }
//    return(rdata);
//}
//
//
//typedef union {
//    uint32_t word;
//    uint8_t byte[4];
//}LongData;
////-----------------------------------------------------
////函数名称： u32 Get_AD_adcIC(void)
////功 能：从 CS1231/CS1232 读取 AD 值数据:24 位
////说 明：返回 4 字节的长整型数据 gdata.word
////-----------------------------------------------------
//uint32_t Get_AD_adcIC(void)
//{
//    u8 i;
//    LongData gdata; //联合体：一个 32 位数据及 4 字节的数组
//    //用于保存从 CS1231/CS1232 读取的 AD 值
//    gdata.byte[0] = 0; //将 32 位数据的最高字节赋值 0
////    while (IO_ADC_DDAT)
//    while( Driver_gpioGet(PORT_SPI_DATA) )
//        ; //读 AD 值之前再次确认数据线是否为低电平
////    PD_CR2 = 0x00; //不使能 PC5 外部中断功能
////    EXTI_CR1 = 0x00; //禁止中断，防止读取 AD 数据时进入中断
////    IO_ADC_SCLK = LOW; //此语句可确保在读 AD 值之前 SCLK 为低电平
//    Driver_gpioSet(PORT_SPI_CLK, PORT_VALUE_LOW);
//    for (i = 1; i < 4; i++) //读到的 3 字节数据到在数组的元素： 1~3
//    {
//        gdata.byte[i] = Read_byte_adcIC();
//    }
////    CS1231_Clk(); //向 CS1231/CS1232 发送结束标志位
//    Driver_CS1231_Clk();
////    PD_CR2 = 0x10; //使能 PC5 外部中断功能
////    EXTI_CR1 |= 0x80; //DDAT 设为输入口，作为中断口
//    return (gdata.word);
//}
////+++++++++++++++++++++++++++++++++++++++++++++
//void Init_adcIC(void) //CS1231/CS1232 初始化
//{
////    IO_ADC_SCLK = LOW; // SCLK 长时间表为高电平时， AD 会进入待机模式
//    Driver_gpioSet(PORT_SPI_CLK, PORT_VALUE_LOW);
////    while (IO_ADC_DDAT)]
//    while(Driver_gpioGet(PORT_SPI_DATA))
//        ; //等待到一个 AD 值，表示 AD 工作正常
////    while (!IO_ADC_DDAT)
//    while(!Driver_gpioGet(PORT_SPI_DATA))
//        ;
//}
//
//////+++++++++++++++++++++++++++++++++++++++++++++
////// 中断服务子程序
////__interrupt void PD2_EXTI_Interrupr(void)
////{
////    B_AdReadEN = 1; //在中断服务子程序中，将“AD 已准备好”的标志置 1
////}
////
//////+++++++++++++++++++++++++++++++++++++++++++++
////// 主程序调用读取 AD 值子程序
////void mail(void)
////{
////    ……
////    if (B_AdReadEN) //在主函数中，判断“AD 已准备好”的标志是否为 1
////    {
////        B_AdReadEN = 0;
////        R_AD = Get_AD_adcIC();
////    ……
////}
////}
