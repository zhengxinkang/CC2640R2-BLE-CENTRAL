/*
 * Driver_ADCmpc3901.c
 *
 *  Created on: 2018��11��14��
 *      Author: zheng
 */

/*******************************************************************
 Microchip 16-24bit ADCоƬ��ȡ����
 �˳���ϵ���ϻ�Դ�����Ա������MCP3901оƬDATASHEET��д
 �˳��������ڱ���16λ/24λADCģ��MCP3901
 2016,1,26����������QQ:704002132
 http://shop57051691.taobao.com
 ������������뱣���������˵��
 *******************************************************************/
//#include <stc12c5a60s2.h>
//#include <intrins.h>
#include "Driver_ADCmpc3901.h"
#include "Driver_simulateSpi.h"
#include "Trace.h"
#include "Driver_gpio.h"
#include "Hal_electricCurrent.h"
#define  uchar unsigned char
#define  uint unsigned int
//#include "mcp3901.h"
//#include "Serial.c"
uchar Adc_dat[3];
/*******************************************************************
 �����ܣ���ʱ����
 ��������
 ********************************************************************/
void delay(uint xx) //��ʱ����
{
    uint a, b;
    for (a = 0; a < 20; a++)
        for (b = xx; b > 0; b--)
            ;
}
/********************************************************************
 �����ܣ���ʼ��MCP3901.
 ������Pga_sel0,Pga_sel1
 �������ܣ�PGA����
 ��������1����Ӧ������������Ϊ+-500MV
 ��������2����Ӧ������������Ϊ+-250MV
 ��������4����Ӧ������������Ϊ+-125MV
 ��������8����Ӧ������������Ϊ+-62.5MV
 ��������16����Ӧ������������Ϊ+-31.25MV
 ��������32����Ӧ������������Ϊ+-15.625MV
 *********************************************************************/
void Init_mcp3901(uchar Pga_sel0, uchar Pga_sel1) //��ʼ��MCP3901
{
    uchar PGAch0_SET, PGAch1_SET;
//    Adc_RST = 0;
    Driver_gpioSet(PORT_ADC_RST,       PORT_VALUE_LOW);
    delay(200);
//    Adc_RST = 1; //Reset MCP3901
    Driver_gpioSet(PORT_ADC_RST,       PORT_VALUE_HIGH);

    Write_byte_mcp3901(CONFIG2, All_shutdown); //����CONFIG2�Ĵ����ض�ģʽ
    Write_byte_mcp3901(CONFIG1, Prescale0 | Osr256 | All_24bit); //����CONFIG1�Ĵ���-��Ԥ��Ƶ����������64������ͨ����Ϊ16λģʽ

    switch (Pga_sel0)
    //ѡ��ͨ��0��PGA����ѹ�Ŵ��ʣ�
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
    //ѡ��ͨ��1��PGA����ѹ�Ŵ��ʣ�
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
    Write_byte_mcp3901(GAIN, PGAch0_SET | PGAch1_SET | AllCH_AmpX2); //���õ�ѹ���棬�������棨�Ŵ����2����ʹ���ڲ�CLOCK����Ҫ�Ŵ�?
    Write_byte_mcp3901(STATUS, Alldata_ok_on_DR | RD_Loop1); //����״̬�Ĵ�������ͨ������OKʱ��DR����������壬
                                                             // ��ַ������������������ȡ�����Ĵ���
    Write_byte_mcp3901(CONFIG2, No_shutdown | All_dither | Vref_out | Clk_xt); //�޹ضϣ�������������
                                                                               //�ⲿ�ο���ѹ,������Ϊʱ��
}

int32_t Read_1CH_adc(uchar ch)
{
    double adda1;
    int32_t adda_int;
    bool isNegative = false;

    if (ch == 0)                                                         //��ȡͨ��0
    {
        Adc_dat[0] = Read_byte_mcp3901(DATA_CH0_0);                    //��ȡ3�ֽ�����
        Adc_dat[1] = Read_byte_mcp3901(DATA_CH0_1);
        Adc_dat[2] = Read_byte_mcp3901(DATA_CH0_2);
    }
    else //��ȡͨ��1
    {
        Adc_dat[0] = Read_byte_mcp3901(DATA_CH1_0);
        Adc_dat[1] = Read_byte_mcp3901(DATA_CH1_1);
        Adc_dat[2] = Read_byte_mcp3901(DATA_CH1_2);
    }

    if (Adc_dat[0] & 0x80) //�жϵ�ѹ����
    {
        //��ѹ�Ǹ���
        Adc_dat[0] = ~Adc_dat[0];
        Adc_dat[1] = ~Adc_dat[1];
        Adc_dat[2] = ~Adc_dat[2];
        isNegative = true;
        //  SendByte('-');
    }
    else
    {
        //  SendByte('+');
    }
    adda1 = (Adc_dat[0] * 65536 + Adc_dat[1] * 256 + Adc_dat[2]);
    adda1 = adda1 * 0.0993; //Ĭ��PGAΪ1����������+-500MV
                            //5000000/((0.5/2.5)*8388608*PGA*3)=0.0993(�ⲿ�ο���ѹ)
                            //5000000/((0.5/2.37)*8388608*PGA*3)=0.0941���ڲ��ο���ѹ��
    adda1 = adda1/GAIN_NUM_ADC;
    adda_int = (int32_t)adda1;
    if(isNegative)
        adda_int = -adda_int;

    return (adda_int);
}

/***************************************************************************
 �����ܣ���������MCP3901оƬ��ȡADCֵ��
 ��������
 ****************************************************************************/
                           //�ڳ�ʼ��MCP3901����������ͨ����PGA


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

