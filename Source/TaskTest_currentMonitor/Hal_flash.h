/*
 * Hal_flash.h
 *
 *  Created on: 2019Äê5ÔÂ8ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_CURRENTMONITOR_HAL_FLASH_H_
#define SOURCE_TASKTEST_CURRENTMONITOR_HAL_FLASH_H_


#include "stdint.h"

#define FLASH_PAGE_NUM                  512 //2MB
#define FLASH_SECTOR_SIZE               0x1000

//#define FLASH_ADDR_TIME                 0x000000
//#define FLASH_ADDR_CURRENT_OFFSET       0x001000
//#define FLASH_ADDR_ABNORMAL_INDEX       0x002000
#define FLASH_ADDR_TEXT_START           0x00000

void        Hal_flash_writeTime(uint32_t timeSecond);
uint32_t    Hal_flash_readTime();

void        Hal_flash_writeCurrentOffset(int32_t offset);
int32_t     Hal_flash_readCurrentOffset();

void        Hal_flash_writeAbnormalIndex(int32_t abnormalIndex);
uint32_t    Hal_flash_readAbnormalIndex();


#endif /* SOURCE_TASKTEST_CURRENTMONITOR_HAL_FLASH_H_ */
