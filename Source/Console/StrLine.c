/*
 * UartDriver.c
 *
 *  Created on: 2017年8月9日
 *      Author: blue.zhengxinkang
 */
#include <StrLine.h>
#include "Hal_UartEmulator.h"
#include "Types.h"
void StrLine_onRecieve(uint8_t* data, uint16_t len);

static OnReceived StrLine_recieveToUp = NULL;

void StrLine_init(void)
{
    Hal_uartEmulator_registeRecieve(StrLine_onRecieve);
}

void StrLine_recieveToUp_setFunc(void (*func)(uint8_t* data, uint16_t len))
{
    StrLine_recieveToUp = func;
}

void StrLine_WriteData(uint8_t* data, uint16_t len)
{
    Hal_uartEmulator_write(data, len);
}

void StrLine_onRecieve(uint8_t* data, uint16_t len)
{
    //进行数据逐个解析，是否一帧
    for(uint32_t index = 0; index < len; index++)
    {
        if('\r'==data[index] && '\n'==data[index+1])
        {   //判断到一帧数据
            if(NULL != StrLine_recieveToUp)
            {
                data[index] = '\0';
                StrLine_recieveToUp(data, index+2);
            }
            else
            {
                //空函数，致命错误
            }
        }
    }
}
