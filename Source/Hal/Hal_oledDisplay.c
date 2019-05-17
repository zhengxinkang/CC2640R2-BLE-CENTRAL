/*
 * Hal_oledDisplay.c
 *
 *  Created on: 2019Äê5ÔÂ11ÈÕ
 *      Author: zheng
 */
#include "Driver_oled.h"
#include "string.h"
#include "BF_Util.h"
#include "VersionInfo.h"

static void Hal_oledDisplay_refresh();
static void Hal_oledDisplay_swVersion();

static uint8_t oledBuf[4][17]=
{
 ' ',  ' ',  ' ',  ' ',   ' ',  ' ',  ' ',  ' ',   ' ',  ' ',  ' ',  ' ',   ' ',  ' ',  ' ',  ' ',  '\0',
 ' ',  ' ',  ' ',  ' ',   ' ',  ' ',  ' ',  ' ',   ' ',  ' ',  ' ',  ' ',   ' ',  ' ',  ' ',  ' ',  '\0',
 ' ',  ' ',  ' ',  ' ',   ' ',  ' ',  ' ',  ' ',   ' ',  ' ',  ' ',  ' ',   ' ',  ' ',  ' ',  ' ',  '\0',
 ' ',  ' ',  ' ',  ' ',   ' ',  ' ',  ' ',  ' ',   ' ',  ' ',  ' ',  ' ',   ' ',  ' ',  ' ',  ' ',  '\0',

// '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  'a',  'b',  'c',  'd',  'e',  'f',  0x00,
// '1',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  'a',  'b',  'c',  'd',  'e',  'f',  0x00,
// '2',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  'a',  'b',  'c',  'd',  'e',  'f',  0x00,
// '3',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  'a',  'b',  'c',  'd',  'e',  'f',  0x00,
};

static uint8_t refreshTimeIndex=0;
static void refresh_timeoutHandler();
#define TIMER_REFRESH_PERIOD      60000

void Hal_oledDisplay_init()
{
    BF_UtilRegeistTimerCallback(refresh_timeoutHandler, &refreshTimeIndex, TIMER_REFRESH_PERIOD);
    Hal_oledDisplay_swVersion();
    refresh_timeoutHandler();
}

static void refresh_timeoutHandler()
{
    OLED_Init();
    OLED_Display_On();

    Hal_oledDisplay_refresh();
    BF_UtilWaitHandle(refreshTimeIndex);
}

void Hal_oledDisplay_num(uint8_t line, uint8_t column, uint32_t num, uint8_t len)
{
    unsigned char t, temp;
    unsigned char enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                oledBuf[line][column+t]=' ';
                continue;
            }
            else
                enshow = 1;
        }
        oledBuf[line][column+t]=temp + '0';
    }

    OLED_ShowNum(column, line*2, num, len);
}

void Hal_oledDisplay_str(uint8_t line, uint8_t column, uint8_t *str)
{
    memcpy(&oledBuf[line][column], str, strlen(str));
    OLED_ShowString(column, line*2, str);
}

void Hal_oledDisplay_refresh()
{
    OLED_ShowString(0, 0, oledBuf[0]);
    OLED_ShowString(0, 2, oledBuf[1]);
    OLED_ShowString(0, 4, oledBuf[2]);
    OLED_ShowString(0, 6, oledBuf[3]);
}

static void Hal_oledDisplay_swVersion()
{
    ManufVersion swVersion = VersionInfo_GetSoftwareVersion();
    Hal_oledDisplay_num(3, 11, swVersion.major, 1);
    Hal_oledDisplay_num(3, 12, swVersion.minor, 1);
    Hal_oledDisplay_num(3, 13, swVersion.revision, 1);
    Hal_oledDisplay_num(3, 14, swVersion.build, 1);
}
