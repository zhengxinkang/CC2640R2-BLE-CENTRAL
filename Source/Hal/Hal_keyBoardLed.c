/*
 * Hal_keyBoardLed.c
 *
 *  Created on: 2018Äê12ÔÂ6ÈÕ
 *      Author: zheng
 */
#include "Hal_expandInput.h"
#include "Trace.h"

char LedSerialToChar(uint8_t serial)
{
    char ch = 'F';
    switch(serial)
    {
        case 0:
        {
            ch = '1';
        }
        break;
        case 1 :
        {
            ch = '2';
        }
        break;
        case 2 :
        {
            ch = '3';
        }
        break;
        case 3 :
        {
            ch = '4';
        }
        break;
        case 4 :
        {
            ch = '5';
        }
        break;
        case 5 :
        {
            ch = '6';
        }
        break;
        case 6 :
        {
            ch = '7';
        }
        break;
        case 7 :
        {
            ch = '8';
        }
        break;
        case 8 :
        {
            ch = '9';
        }
        break;
        case 9 :
        {
            ch = '*';
        }
        break;
        case 10 :
        {
            ch = '0';
        }
        break;
        case 11 :
        {
            ch = '#';
        }
        break;
        default:
        {
            ch = 'F';
            TRACE_ERROR("LedCharToSerial: Serial error!\n");
        }
    }
    return ch;
}

EXPAND_INPUT_SERIAL LedSerialToCode(uint8_t serial)
{
    EXPAND_INPUT_SERIAL code;
    switch(serial)
    {
        case 0:
        {
            code = R_LED1;
        }
        break;
        case 1:
        {
            code = R_LED2;
        }
        break;
        case 2:
        {
            code = R_LED3;
        }
        break;
        case 3:
        {
            code = R_LED4;
        }
        break;
        case 4:
        {
            code = R_LED5;
        }
        break;
        case 5:
        {
            code = R_LED6;
        }
        break;
        case 6:
        {
            code = R_LED7;
        }
        break;
        case 7:
        {
            code = R_LED8;
        }
        break;
        case 8:
        {
            code = R_LED9;
        }
        break;
        case 9:
        {
            code = R_LED10;
        }
        break;
        case 10:
        {
            code = R_LED11;
        }
        break;
        case 11:
        {
            code = R_LED12;
        }
        break;
        default:
        {
            code = INPUT_SERIAL_ERROR;
            TRACE_ERROR("LedSerialToCode: serial error!\n");
        }
    }
    return code;
}
