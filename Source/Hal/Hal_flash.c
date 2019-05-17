/*
 * Hal_flash.c
 *
 *  Created on: 2019Äê5ÔÂ8ÈÕ
 *      Author: zheng
 */
#include "stdint.h"
#include "Hal_flash.h"
#include "Driver_internalFlash.h"

//----------------------------------------------------
void        Hal_flash_writeTime(uint32_t timeSecond)
{
    flashParam.time=timeSecond;
    flashParamParamWrite();

}

uint32_t    Hal_flash_readTime()
{
    flashParamParamRead();
    return flashParam.time;
}

//---
void        Hal_flash_writeCurrentOffset(int32_t offset)
{
    flashParam.currentOffset=offset;
    flashParamParamWrite();
}

int32_t     Hal_flash_readCurrentOffset()
{
    flashParamParamRead();
    return flashParam.currentOffset;
}

//---
void        Hal_flash_writeAbnormalIndex(int32_t abnormalIndex)
{
    flashParam.abnormalIndex=abnormalIndex;
    flashParamParamWrite();
}

uint32_t    Hal_flash_readAbnormalIndex()
{
    flashParamParamRead();
    return flashParam.abnormalIndex;
}
