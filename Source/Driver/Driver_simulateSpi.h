/*
 * Driver_simulateSpi.h
 *
 *  Created on: 2018��11��14��
 *      Author: zheng
 */

#ifndef SOURCE_DRIVER_DRIVER_SIMULATESPI_H_
#define SOURCE_DRIVER_DRIVER_SIMULATESPI_H_


#include "Driver_gpio.h"
//#include <intrins.h>
#define uchar   unsigned char
 //CHANNEL ADDRESS
#define DATA_CH0_0 0X00//ͨ��0��1�����ݼĴ�����ַ
#define DATA_CH0_1 0X01//ͨ��0��2�����ݼĴ�����ַ
#define DATA_CH0_2 0X02//ͨ��0��3�����ݼĴ�����ַ

#define DATA_CH1_0 0X03//ͨ��1��1�����ݼĴ�����ַ
#define DATA_CH1_1 0X04//ͨ��1��2�����ݼĴ�����ַ
#define DATA_CH1_2 0X05//ͨ��1��3�����ݼĴ�����ַ

#define MOD      0X06  //����������Ĵ�����ַ
#define PHASE    0X07  //��λ�ӳټĴ�����ַ
#define GAIN     0X08  //�������üĴ���
#define STATUS   0X09  //״̬/ͨ�żĴ���
#define CONFIG1  0X0A  //���üĴ��� 1
#define CONFIG2  0X0B  //���üĴ��� 2

//GAIN SET
#define CH0_PGA_1       0X00//CH0��������1����Ӧ������������Ϊ+-500MV
#define CH0_PGA_2       0X01//CH0��������2����Ӧ������������Ϊ+-250MV
#define CH0_PGA_4       0X02//CH0��������4����Ӧ������������Ϊ+-125MV
#define CH0_PGA_8       0X03//CH0��������8����Ӧ������������Ϊ+-62.5MV
#define CH0_PGA_16      0X04//CH0��������16����Ӧ������������Ϊ+-31.25MV
#define CH0_PGA_32      0X05//CH0��������32����Ӧ������������Ϊ+-15.625MV

//���ٹ���ʱ�����Ŵ����
#define  AllCH_AmpX2    0X18//����ͨ�������Ŵ�2��
#define  CH1_AmpX2      0X10//CH1ͨ�������Ŵ�2��
#define  CH0_AmpX2      0X08//CH0ͨ�������Ŵ�2��
#define  NOCH_AmpX2     0X00//��ͨ�������Ŵ�

#define CH1_PGA_1       0X00//CH1��������1����Ӧ������������Ϊ+-500MV
#define CH1_PGA_2       0X20//CH1��������2����Ӧ������������Ϊ+-250MV
#define CH1_PGA_4       0X40//CH1��������4����Ӧ������������Ϊ+-125MV
#define CH1_PGA_8       0X60//CH1��������8����Ӧ������������Ϊ+-62.5MV
#define CH1_PGA_16      0X80//CH1��������16����Ӧ������������Ϊ+-31.25MV
#define CH1_PGA_32      0Xa0//CH1��������32����Ӧ������������Ϊ+-15.625MV

//STATUS REG
#define  RD_Loop1    0X00//��ַ������������������ȡ�����Ĵ���
#define  RD_Loop2    0X20//�Ĵ������͵ĵ�ַ������ѭ�� ��Ĭ��ֵ��

#define  Alldata_ok_on_DR    0x0c//ADCͨ��0��ADCͨ��1�����ݾ�������ͬʱ�����DR���š�
#define  Ch0data_ok_on_DR    0x04//ADCͨ��0�����ݾ������������DR���š�ADCͨ��1��DR�ź�δ�����ڴ����š�
#define  Ch1data_ok_on_DR    0x08//ADCͨ��1�����ݾ������������DR����,ADCͨ��0��DR�ź�δ�����ڴ����š�
#define  Delaydata_ok_on_DR  0x00//����ADC���ͺ��ADCͨ�������ݾ������������DR���š��ͺ�ADC��ѡ��ȡ������λ�Ĵ���
                                 //�� OSR ���� ��Ĭ��ֵ����
//CONFIG REG1(���üĴ���)
// �ڲ���ʱ�� �� AMCLK��Ԥ��Ƶֵ
#define  Prescale0   0x00 // AMCLK = MCLK
#define  Prescale2   0x40 // AMCLK = MCLK/2
#define  Prescale4   0x80 // AMCLK = MCLK/4
#define  Prescale8   0xc0 // AMCLK = MCLK/8

//��������
#define  Osr32       0x00
#define  Osr64       0x10//default
#define  Osr128      0x20
#define  Osr256      0x30

#define  All_24bit             0x0c//����ͨ������Ϊ24λģʽ
#define  All_16bit             0x00//����ͨ������Ϊ16λģʽ
#define  Ch0_16bit_Ch1_24bit   0x08//CH0����16λ��CH1����24λ
#define  Ch1_16bit_Ch0_24bit   0x04//CH1����16λ��CH0����24λ

#define  Md_out                0x00 //�޵����������ʹ�ܣ�Ĭ��ֵ��

//CONFIG REG2
#define  No_shutdown      0x00//�޹ض�ģʽ
#define  Ch0_shutdown     0x10//CH0�ض�ģʽ
#define  Ch1_shutdown     0x20//CH1�ض�ģʽ
#define  All_shutdown     0x30//ȫ�����ùض�ģʽ

#define  No_dither         0x00//�޶�������
#define  Ch0_dither        0x04//CH0��������
#define  Ch1_dither        0x08//CH1��������
#define  All_dither        0x0c//ȫ�����ö�������

#define  Vref_in           0x00//ʹ���ڲ��ο���ѹ
#define  Vref_out          0x02//ʹ���ⲿ�ο���ѹ

#define  Clk_out           0x01//ʹ���ⲿʱ���ź�
#define  Clk_xt            0x00//ʹ�þ���ʱ���ź�

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//sbit Adc_DR=P2^0;//�������ݾ�������
//sbit Adc_CS=P2^1; //Ƭѡ����
//sbit Adc_SCK=P2^2;//ʱ������
//sbit Adc_SDO=P2^3;//���нӿ������������
//sbit Adc_SDI=P2^4;//���нӿ�������������
//sbit Adc_RST=P2^5;//����оƬ��λ����

//sbit Led1=P0^0;//����һ��LED,���ڹ���ָʾ
//sbit Led2=P0^1;//����һ��LED,���ڹ���ָʾ
//sbit Led3=P0^2;//����һ��LED,���ڹ���ָʾ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//SPI MODE 0,0
void Write_byte_mcp3901(uchar addr, uchar dat); //��MCP3901оƬָ���ĵ�ַд������
uchar Read_byte_mcp3901(uchar addr); //��MCP3901оƬ�ƶ��ĵ�ַ��������


#endif /* SOURCE_DRIVER_DRIVER_SIMULATESPI_H_ */
