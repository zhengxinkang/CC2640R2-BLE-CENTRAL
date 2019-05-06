/*
 * Hal_oled.h
 *
 *  Created on: 2018Äê11ÔÂ24ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_HAL_HAL_OLED_H_
#define SOURCE_HAL_HAL_OLED_H_


#include "stdint.h"
typedef enum{
    OLED_DATA_TYPE_ITEM                 ,
    OLED_DATA_TYPE_LASTTIME             ,
    OLED_DATA_TYPE_THISTIME             ,
    OLED_DATA_TYPE_REASON               ,
}OLED_DATA_TYPE;

typedef enum{
    OLED_STR_TYPE_ITEM                 ,
    OLED_STR_TYPE_LASTTIME             ,
    OLED_STR_TYPE_THISTIME             ,
}OLED_STR_TYPE;
void Hal_oled_init();
void Hal_oled_currentChange(uint32_t avgUa, uint32_t thisUa);
void Hal_oled_strChange(OLED_STR_TYPE strType, uint8_t* str);
void Hal_oled_dataChange(OLED_DATA_TYPE strType, uint32_t data);


#endif /* SOURCE_HAL_HAL_OLED_H_ */
