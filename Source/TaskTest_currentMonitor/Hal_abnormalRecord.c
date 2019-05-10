/*
 * Hal_abnormalRecord.c
 *
 *  Created on: 2019年5月8日
 *      Author: zheng
 */

#include "Hal_flash.h"
#include "w25qxx.h"
#include "stdbool.h"
#include "Driver_spi.h"
#include "Hal_oled.h"
#include "Trace.h"
#include "Hal_abnormalRecord.h"
#include "UTC_clock.h"

void Hal_abnormalRecord_init()
{
//    W25QXX_Erase_Chip();
    extFlash_eraseChip();
    Hal_flash_writeAbnormalIndex(0x00);
    Hal_oled_abnormalCount(Hal_abnormalRecord_indexRead());

    UTCTime second = 0;
    UTCTimeStruct s_time;
    UTC_convertUTCTime(&s_time, 0);
    Hal_flash_writeTime(second);
    UTC_setClock(second);
    Hal_oled_timeChange(s_time);
}

uint32_t Hal_abnormalRecord_indexRead()
{
    uint32_t index = Hal_flash_readAbnormalIndex();
    if(index == 0xffffffff)
        index=0;
    return index;
}

bool Hal_abnormalRecord_indexWrite()
{
    uint32_t index = Hal_flash_readAbnormalIndex();

    if(index>=48)
    {
        TRACE_DEBUG("Flash 已经存储满了，无法写入更多内容！\n");
        return false;
    }
    else
    {
        index++;
        TRACE_DEBUG("开始向 Flash 写入第 %d 个异常记录。\n", index);
        Hal_oled_abnormalCount(index);
        Hal_flash_writeAbnormalIndex(index);
        return true;
    }
}

void Hal_abnormal_recordText(uint8_t status_record, uint32_t* current, uint32_t currentAvg, uint32_t second)
{
    uint32_t index = Hal_flash_readAbnormalIndex();

    uint64_t addr = FLASH_ADDR_TEXT_START +(index-1)*10*FLASH_SECTOR_SIZE +(status_record-1)*0x100;
//    W25QXX_Write_NoCheck(current,addr,50*4);
//    W25QXX_Write_NoCheck(&currentAvg,addr+200,4);
//    W25QXX_Write_NoCheck(&second,addr+204,4);
    extFlash_write(addr, 50*4, current);
    extFlash_write(addr+200, 4, &currentAvg);
    extFlash_write(addr+204, 4, &second);

}
