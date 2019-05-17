/*
 * Hal_oled.c
 *
 *  Created on: 2018Äê11ÔÂ24ÈÕ
 *      Author: zheng
 */
#include "Driver_oled.h"
#include "Hal_oled.h"
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "Hal_oledDisplay.h"


void Hal_oled_currentChange(uint32_t avgUa, uint32_t thisUa)
{
    Hal_oledDisplay_str(0,0,"i=");
    if(avgUa < 1000)
    {
        Hal_oledDisplay_num(0, 3, avgUa, 3);
        Hal_oledDisplay_str(0,7,"u");
    }
    else
    {
        Hal_oledDisplay_num(0, 3, avgUa/1000, 3);
        Hal_oledDisplay_str(0,7,"m");
    }

    if(thisUa < 1000)
    {
        Hal_oledDisplay_num(0, 9, thisUa, 3);
        Hal_oledDisplay_str(0,13,"u");
    }
    else
    {
        Hal_oledDisplay_num(0, 9, thisUa/1000, 3);
        Hal_oledDisplay_str(0,13,"m");
    }
}

void Hal_oled_strChange(OLED_STR_TYPE strType, uint8_t* str)
{
//    BF_taskSleepMs(50);
    switch(strType)
    {
        case OLED_STR_TYPE_ITEM:
        {
            Hal_oledDisplay_str(1,2,str);
        }
        break;
        case OLED_STR_TYPE_LASTTIME:
        {
            Hal_oledDisplay_str(2,0,str);
        }
        break;
        case OLED_STR_TYPE_THISTIME:
        {
            Hal_oledDisplay_str(3,0,str);
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
            Hal_oledDisplay_num(1, 0, data, 2);
        }
        break;
        case OLED_DATA_TYPE_LASTTIME:
        {
            Hal_oledDisplay_num(2, 7, data, 3);
        }
        break;
        case OLED_DATA_TYPE_THISTIME:
        {
            Hal_oledDisplay_num(3, 7, data, 3);
        }
        case OLED_DATA_TYPE_REASON:
        {
            Hal_oledDisplay_num(3, 12, data, 3);
        }
        break;
        case OLED_DATA_TYPE_CURRENT:
        {
            Hal_oledDisplay_num(0, 2, data, 6);
        }
        break;
    }
}

void Hal_oled_abnormalCount(uint32_t count)
{
    Hal_oledDisplay_str(2,0,"error:");
    Hal_oledDisplay_num(2, 7, count, 4);
}

void Hal_oled_timeChange(UTCTimeStruct s_time)
{
    Hal_oledDisplay_num(3, 0, s_time.month+1, 2);
    Hal_oledDisplay_str(3, 2,"-");
    Hal_oledDisplay_num(3, 3, s_time.day+1, 2);

    Hal_oledDisplay_num(3, 7, s_time.hour, 2);
    Hal_oledDisplay_str(3, 9,"-");
    Hal_oledDisplay_num(3, 10, s_time.minutes, 2);
    Hal_oledDisplay_str(3, 12,"-");
    Hal_oledDisplay_num(3, 13, s_time.seconds, 2);
}
