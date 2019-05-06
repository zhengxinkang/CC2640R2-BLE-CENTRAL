/*
 * KeyBoard_action.c
 *
 *  Created on: 2018Äê11ÔÂ23ÈÕ
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "Trace.h"
#include "Hal_expandOutput.h"
#include <string.h>
#include <BF_Util.h>

uint8_t KeyBoard_wake(void)
{
    Hal_expandOutput(KEY_1, PORT_VALUE_HIGH);
    Hal_expandOutput(KEY_2, PORT_VALUE_HIGH);
    Hal_expandOutput(KEY_3, PORT_VALUE_HIGH);
    Hal_expandOutput(KEY_4, PORT_VALUE_HIGH);
    Hal_expandOutput(KEY_5, PORT_VALUE_HIGH);
    Hal_expandOutput(KEY_6, PORT_VALUE_HIGH);
    Hal_expandOutput(KEY_7, PORT_VALUE_HIGH);
    Hal_expandOutput(KEY_8, PORT_VALUE_HIGH);
    Hal_expandOutput(KEY_9, PORT_VALUE_HIGH);
    Hal_expandOutput(KEY_10, PORT_VALUE_HIGH);
    Hal_expandOutput(KEY_11, PORT_VALUE_HIGH);
    Hal_expandOutput(KEY_12, PORT_VALUE_HIGH);
    BF_taskSleepMs( 100 );
    Hal_expandOutput(KEY_1, PORT_VALUE_LOW);
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

    return 0;
}

EXPAND_OUTPUT_SERIAL charToKeyserail(char ch)
{
    EXPAND_OUTPUT_SERIAL serial;
    switch(ch)
    {
        case '1':
        {
            serial = KEY_1;
        }
        break;
        case '2':
        {
            serial = KEY_2;
        }
        break;
        case '3':
        {
            serial = KEY_3;
        }
        break;
        case '4':
        {
            serial = KEY_4;
        }
        break;
        case '5':
        {
            serial = KEY_5;
        }
        break;
        case '6':
        {
            serial = KEY_6;
        }
        break;
        case '7':
        {
            serial = KEY_7;
        }
        break;
        case '8':
        {
            serial = KEY_8;
        }
        break;
        case '9':
        {
            serial = KEY_9;
        }
        break;
        case '*':
        {
            serial = KEY_10;
        }
        break;
        case '0':
        {
            serial = KEY_11;
        }
        break;
        case '#':
        {
            serial = KEY_12;
        }
        break;
        default:
        {
            serial = SERIAL_ERROR;
            TRACE_ERROR("charToKeyserail: Serial error!\n");
        }
    }
    return serial;
}

uint8_t KeyBoard_stringAction(char* keyCodeString, uint16_t interval, uint16_t duration)
{
    uint8_t keyCodeLen = strlen(keyCodeString);
    for(uint8_t i = 0;i<keyCodeLen;i++)
    {
        EXPAND_OUTPUT_SERIAL serial = charToKeyserail(keyCodeString[i]);
        if(SERIAL_ERROR != serial)
        {
            Hal_expandOutput(serial, PORT_VALUE_HIGH);
            BF_taskSleepMs( interval );
            Hal_expandOutput(serial, PORT_VALUE_LOW);
        }
        BF_taskSleepMs( duration );
    }
    return 0;
}

uint8_t KeyBoard_oneAction(EXPAND_OUTPUT_SERIAL serial)
{
    uint8_t keyNum = 11-serial;
    Hal_expandOutput(keyNum, PORT_VALUE_HIGH);
    BF_taskSleepMs( 100 );
    Hal_expandOutput(keyNum, PORT_VALUE_LOW);
    return 0;
}

uint8_t KeyBoard_fastSleep()
{
    KeyBoard_oneAction(KEY_10);
    return 0;
}
