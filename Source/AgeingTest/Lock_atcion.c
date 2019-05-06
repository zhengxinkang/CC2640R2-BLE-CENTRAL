/*
 * Lock_atcion.c
 *
 *  Created on: 2018��11��23��
 *      Author: zheng
 */
#include "BF_Util.h"
#include "Hal_expandOutput.h"

void Lock_action_init(void)
{
    //KEY 1 �ߵ�ƽ��Ч   Ĭ�ϣ��͵�ƽ
    Hal_expandOutput(KEY_1, PORT_VALUE_LOW);            //���� ����Ч                    Ĭ�ϣ���
    Hal_expandOutput(KEY_2, PORT_VALUE_LOW);
    Hal_expandOutput(KEY_3, PORT_VALUE_LOW);
    Hal_expandOutput(KEY_4, PORT_VALUE_LOW);
    Hal_expandOutput(KEY_5, PORT_VALUE_LOW);
    Hal_expandOutput(KEY_6, PORT_VALUE_LOW);
    Hal_expandOutput(KEY_7, PORT_VALUE_LOW);
    Hal_expandOutput(KEY_8, PORT_VALUE_LOW);
    Hal_expandOutput(KEY_9, PORT_VALUE_LOW);
    Hal_expandOutput(KEY_10, PORT_VALUE_LOW);
    Hal_expandOutput(KEY_11, PORT_VALUE_LOW);
    Hal_expandOutput(KEY_12, PORT_VALUE_LOW);
    Hal_expandOutput(CLMA, PORT_VALUE_LOW);             //ˢ�� ����Ч                    Ĭ�ϣ���
    Hal_expandOutput(CLMB, PORT_VALUE_LOW);
    Hal_expandOutput(PWR_SW_YJ, PORT_VALUE_HIGH);       //Ӧ�� ����Ч    Ĭ�ϣ���
    Hal_expandOutput(PWR_SW_BAT, PORT_VALUE_HIGH);      //��� ����Ч    Ĭ�ϣ���
    Hal_expandOutput(LATCH_BOLT, PORT_VALUE_HIGH);      //���� ����Ч    Ĭ�ϣ���
    Hal_expandOutput(USR_SET, PORT_VALUE_HIGH);         //���� ����Ч    Ĭ�ϣ���
    Hal_expandOutput(USR_CLR, PORT_VALUE_HIGH);         //��� ����Ч    Ĭ�ϣ���
    Hal_expandOutput(USR_RELOCK, PORT_VALUE_HIGH);      //���� ����Ч    Ĭ�ϣ���
    Hal_expandOutput(FQ_SW, PORT_VALUE_LOW);            //���� ����Ч                    Ĭ�ϣ���
    Hal_expandOutput(LED_Y, PORT_VALUE_HIGH);           //LED1 ����Ч    Ĭ�ϣ���
    Hal_expandOutput(LED_G, PORT_VALUE_HIGH);           //LED2 ����Ч    Ĭ�ϣ���
    Hal_expandOutput(LED_R, PORT_VALUE_HIGH);           //LED3 ����Ч    Ĭ�ϣ���
}

void Lock_action_bolt(uint16_t ms)
{
    Hal_expandOutput(LATCH_BOLT, PORT_VALUE_LOW);
    BF_taskSleepMs( ms );
    Hal_expandOutput(LATCH_BOLT, PORT_VALUE_HIGH);
}

void Lock_action_buttonSet()
{
    Hal_expandOutput(USR_SET, PORT_VALUE_LOW);
    BF_taskSleepMs( 200 );
    Hal_expandOutput(USR_SET, PORT_VALUE_HIGH);
}

void Lock_action_buttonClr()
{
    Hal_expandOutput(USR_CLR, PORT_VALUE_LOW);
    BF_taskSleepMs( 6000 );
    Hal_expandOutput(USR_CLR, PORT_VALUE_HIGH);
}

void Lock_action_relock(bool isLocked)
{
    Hal_expandOutput(USR_RELOCK, (PortValue)!isLocked);
}

void Lock_action_fq(bool isActive)
{
    Hal_expandOutput(FQ_SW, (PortValue)!isActive);
}

void Lock_action_power(bool powerBat, bool powerUsb)
{
    Hal_expandOutput(PWR_SW_BAT, (PortValue)!powerBat);
    Hal_expandOutput(PWR_SW_YJ, (PortValue)!powerUsb);
}

void Lock_action_cardActive(uint16_t ms)
{
    Hal_expandOutput(CLMA, PORT_VALUE_HIGH);
    BF_taskSleepMs( ms );
    Hal_expandOutput(CLMA, PORT_VALUE_LOW);
}

