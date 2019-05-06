///*
// * Driver_CS1231.c
// *
// *  Created on: 2018��11��23��
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
////�������ƣ� void Driver_CS1231_Clk(void)
////�� �ܣ�ʱ������
////˵ ����
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
////�������ƣ��� CS1231/CS1232 ��ȡ 1 �ֽڵ�����
////�� �ܣ�
////˵ �������� 1 �ֽڵ��޷����ַ��� rdata
////-----------------------------------------------------
//u8 Read_byte_adcIC(void)
//{
//    u8 i;
//    u8 rdata = 0; //��ʼ������ֵΪ 0
//    for(i = 0; i < 8; i++)
//    {
//        rdata <<= 1; //����ֵ���� 1 λ
////        CS1231_Clk();//�����ؽ�������,�½��ظ�������
//        Driver_CS1231_Clk();
////        if(IO_ADC_DDAT)
//        if( Driver_gpioGet(PORT_SPI_DATA) )
//        {
//            rdata += 1; //��������������Ϊ 1���򷵻�ֵ�� 1
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
////�������ƣ� u32 Get_AD_adcIC(void)
////�� �ܣ��� CS1231/CS1232 ��ȡ AD ֵ����:24 λ
////˵ �������� 4 �ֽڵĳ��������� gdata.word
////-----------------------------------------------------
//uint32_t Get_AD_adcIC(void)
//{
//    u8 i;
//    LongData gdata; //�����壺һ�� 32 λ���ݼ� 4 �ֽڵ�����
//    //���ڱ���� CS1231/CS1232 ��ȡ�� AD ֵ
//    gdata.byte[0] = 0; //�� 32 λ���ݵ�����ֽڸ�ֵ 0
////    while (IO_ADC_DDAT)
//    while( Driver_gpioGet(PORT_SPI_DATA) )
//        ; //�� AD ֵ֮ǰ�ٴ�ȷ���������Ƿ�Ϊ�͵�ƽ
////    PD_CR2 = 0x00; //��ʹ�� PC5 �ⲿ�жϹ���
////    EXTI_CR1 = 0x00; //��ֹ�жϣ���ֹ��ȡ AD ����ʱ�����ж�
////    IO_ADC_SCLK = LOW; //������ȷ���ڶ� AD ֵ֮ǰ SCLK Ϊ�͵�ƽ
//    Driver_gpioSet(PORT_SPI_CLK, PORT_VALUE_LOW);
//    for (i = 1; i < 4; i++) //������ 3 �ֽ����ݵ��������Ԫ�أ� 1~3
//    {
//        gdata.byte[i] = Read_byte_adcIC();
//    }
////    CS1231_Clk(); //�� CS1231/CS1232 ���ͽ�����־λ
//    Driver_CS1231_Clk();
////    PD_CR2 = 0x10; //ʹ�� PC5 �ⲿ�жϹ���
////    EXTI_CR1 |= 0x80; //DDAT ��Ϊ����ڣ���Ϊ�жϿ�
//    return (gdata.word);
//}
////+++++++++++++++++++++++++++++++++++++++++++++
//void Init_adcIC(void) //CS1231/CS1232 ��ʼ��
//{
////    IO_ADC_SCLK = LOW; // SCLK ��ʱ���Ϊ�ߵ�ƽʱ�� AD ��������ģʽ
//    Driver_gpioSet(PORT_SPI_CLK, PORT_VALUE_LOW);
////    while (IO_ADC_DDAT)]
//    while(Driver_gpioGet(PORT_SPI_DATA))
//        ; //�ȴ���һ�� AD ֵ����ʾ AD ��������
////    while (!IO_ADC_DDAT)
//    while(!Driver_gpioGet(PORT_SPI_DATA))
//        ;
//}
//
//////+++++++++++++++++++++++++++++++++++++++++++++
////// �жϷ����ӳ���
////__interrupt void PD2_EXTI_Interrupr(void)
////{
////    B_AdReadEN = 1; //���жϷ����ӳ����У�����AD ��׼���á��ı�־�� 1
////}
////
//////+++++++++++++++++++++++++++++++++++++++++++++
////// ��������ö�ȡ AD ֵ�ӳ���
////void mail(void)
////{
////    ����
////    if (B_AdReadEN) //���������У��жϡ�AD ��׼���á��ı�־�Ƿ�Ϊ 1
////    {
////        B_AdReadEN = 0;
////        R_AD = Get_AD_adcIC();
////    ����
////}
////}
