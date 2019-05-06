/*
 * Hal_uart.c
 *
 *  Created on: 2018年7月5日
 *      Author: zheng
 */
#include <stdarg.h>               /*支持函数接收不定量参数*/
#include "Driver_UartEmulator.h"


const char * const g_pcHex = "0123456789abcdef";

void Hal_uartEmulator_registeRecieve( void (*func)(uint8_t* data, uint16_t len) )
{
    Driver_uartEmulator_registeRecieve(func);
}

void Hal_uartEmulator_write(uint8_t *pcBuf, uint16_t ulLen)
{
    Driver_uartEmulator_sendChars((uint8_t*)pcBuf, ulLen);
}

///**
// * 简介:   一个简单的printf函数,支持\%c, \%d, \%p, \%s, \%u,\%x, and \%X.
// */
//void Hal_uartEmulator_printf(const char *pcString, ...)
//{
//    uint32_t ulIdx;
//    uint32_t ulValue;       //保存从不定量参数堆栈中取出的数值型变量
//    uint32_t ulPos, ulCount;
//    uint32_t ulBase;        //保存进制基数,如十进制则为10,十六进制数则为16
//    uint32_t ulNeg;         //为1表示从变量为负数
//    char *pcStr;         //保存从不定量参数堆栈中取出的字符型变量
//    char pcBuf[32];      //保存数值型变量字符化后的字符
//    uint8_t cFill;          //'%08x'->不足8个字符用'0'填充,cFill='0';
//                            //'%8x '->不足8个字符用空格填充,cFill=' '
//    va_list vaArgP;
//
//    va_start(vaArgP, pcString);
//    while (*pcString)
//    {
//        // 首先搜寻非%核字符串结束字符
//        for (ulIdx = 0; (pcString[ulIdx] != '%') && (pcString[ulIdx] != '\0');
//                ulIdx++)
//        {
//        }
//        Hal_uartEmulator_write(pcString, ulIdx);
//
//        pcString += ulIdx;
//        if (*pcString == '%')
//        {
//            pcString++;
//
//            ulCount = 0;
//            cFill = ' ';
//            again: switch (*pcString++)
//            {
//            case '0':
//            case '1':
//            case '2':
//            case '3':
//            case '4':
//            case '5':
//            case '6':
//            case '7':
//            case '8':
//            case '9':
//            {
//                // 如果第一个数字为0, 则使用0做填充,则用空格填充)
//                if ((pcString[-1] == '0') && (ulCount == 0))
//                {
//                    cFill = '0';
//                }
//                ulCount *= 10;
//                ulCount += pcString[-1] - '0';
//                goto again;
//            }
//            case 'c':
//            {
//                ulValue = va_arg(vaArgP, unsigned long);
//                Hal_uartEmulator_write((char *) &ulValue, 1);
//                break;
//            }
//            case 'd':
//            {
//                ulValue = va_arg(vaArgP, unsigned long);
//                ulPos = 0;
//
//                if ((long) ulValue < 0)
//                {
//                    ulValue = -(long) ulValue;
//                    ulNeg = 1;
//                }
//                else
//                {
//                    ulNeg = 0;
//                }
//                ulBase = 10;
//                goto convert;
//            }
//            case 's':
//            {
//                pcStr = va_arg(vaArgP, char *);
//
//                for (ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++)
//                {
//                }
//                Hal_uartEmulator_write(pcStr, ulIdx);
//
//                if (ulCount > ulIdx)
//                {
//                    ulCount -= ulIdx;
//                    while (ulCount--)
//                    {
//                        Hal_uartEmulator_write(" ", 1);
//                    }
//                }
//                break;
//            }
//            case 'u':
//            {
//                ulValue = va_arg(vaArgP, unsigned long);
//                ulPos = 0;
//                ulBase = 10;
//                ulNeg = 0;
//                goto convert;
//            }
//            case 'x':
//            case 'X':
//            case 'p':
//            {
//                ulValue = va_arg(vaArgP, unsigned long);
//                ulPos = 0;
//                ulBase = 16;
//                ulNeg = 0;
//                convert:   //将数值转换成字符
//                for (ulIdx = 1;
//                        (((ulIdx * ulBase) <= ulValue)
//                                && (((ulIdx * ulBase) / ulBase) == ulIdx));
//                        ulIdx *= ulBase, ulCount--)
//                {
//                }
//                if (ulNeg)
//                {
//                    ulCount--;
//                }
//                if (ulNeg && (cFill == '0'))
//                {
//                    pcBuf[ulPos++] = '-';
//                    ulNeg = 0;
//                }
//                if ((ulCount > 1) && (ulCount < 16))
//                {
//                    for (ulCount--; ulCount; ulCount--)
//                    {
//                        pcBuf[ulPos++] = cFill;
//                    }
//                }
//
//                if (ulNeg)
//                {
//                    pcBuf[ulPos++] = '-';
//                }
//
//                for (; ulIdx; ulIdx /= ulBase)
//                {
//                    pcBuf[ulPos++] = g_pcHex[(ulValue / ulIdx) % ulBase];
//                }
//                Hal_uartEmulator_write(pcBuf, ulPos);
//                break;
//            }
//            case '%':
//            {
//                Hal_uartEmulator_write(pcString - 1, 1);
//                break;
//            }
//            default:
//            {
//                Hal_uartEmulator_write("ERROR", 5);
//                break;
//            }
//            }
//        }
//    }
//    //可变参数处理结束
//    va_end(vaArgP);
//}



