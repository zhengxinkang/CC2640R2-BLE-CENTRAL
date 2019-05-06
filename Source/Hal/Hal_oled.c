/*
 * Hal_oled.c
 *
 *  Created on: 2018��11��24��
 *      Author: zheng
 */
#include "Driver_oled.h"
#include "Hal_oled.h"
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

void Hal_oled_init()
{
    OLED_Init();
    OLED_Display_On();
//����        0123456789012345
//��һ�У� i= ddd m ddd m
//�ڶ��У� ddd  - clearBtn
//�����У� good  ddd
//�����У� PASS  ddd
    OLED_ShowNum(0, 2, 00, 2);      OLED_ShowString(0,2,"              ");
    OLED_ShowString(0,4,"       ");    OLED_ShowNum(7, 4, 000, 3);
    OLED_ShowString(0,6,"       ");    OLED_ShowNum(7, 6, 000, 3);
}

void Hal_oled_currentChange(uint32_t avgUa, uint32_t thisUa)
{
//    BF_taskSleepMs(50);
    OLED_ShowString(0,0,"i=");
    if(avgUa < 1000)
    {
        OLED_ShowNum(3, 0, avgUa, 3);
        OLED_ShowString(7,0,"u");
    }
    else
    {
        OLED_ShowNum(3, 0, avgUa/1000, 3);
        OLED_ShowString(7,0,"m");
    }

    if(thisUa < 1000)
    {
        OLED_ShowNum(9, 0, thisUa, 3);
        OLED_ShowString(13,0,"u");
    }
    else
    {
        OLED_ShowNum(9, 0, thisUa/1000, 3);
        OLED_ShowString(13,0,"m");
    }
}

void Hal_oled_strChange(OLED_STR_TYPE strType, uint8_t* str)
{
//    BF_taskSleepMs(50);
    switch(strType)
    {
        case OLED_STR_TYPE_ITEM:
        {
            OLED_ShowString(2,2,str);
        }
        break;
        case OLED_STR_TYPE_LASTTIME:
        {
            OLED_ShowString(0,4,str);
        }
        break;
        case OLED_STR_TYPE_THISTIME:
        {
            OLED_ShowString(0,6,str);
        }
        break;
    }
}

void Hal_oled_dataChange(OLED_DATA_TYPE strType, uint32_t data)
{
//    BF_taskSleepMs(50);
    switch(strType)
    {
        case OLED_DATA_TYPE_ITEM:
        {
            OLED_ShowNum(0, 2, data, 2);
        }
        break;
        case OLED_DATA_TYPE_LASTTIME:
        {
            OLED_ShowNum(7, 4, data, 3);
        }
        break;
        case OLED_DATA_TYPE_THISTIME:
        {
            OLED_ShowNum(7, 6, data, 3);
        }
        case OLED_DATA_TYPE_REASON:
        {
            OLED_ShowNum(12, 6, data, 3);
        }
        break;
    }
}

