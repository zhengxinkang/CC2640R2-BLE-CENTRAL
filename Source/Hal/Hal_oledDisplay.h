/*
 * Hal_oledDisplay.h
 *
 *  Created on: 2019年5月11日
 *      Author: zheng
 */

#ifndef SOURCE_HAL_HAL_OLEDDISPLAY_H_
#define SOURCE_HAL_HAL_OLEDDISPLAY_H_

/* func:
 *      显示数字
 * para:
 *      line    -行
 *      column  -偏移
 *      num     -数据
 *      len     -长度
 * */
void Hal_oledDisplay_num(uint8_t line, uint8_t column, uint32_t num, uint8_t len);

/* func:
 *      显示字符串
 * para:
 *      line    -行
 *      column  -偏移
 *      str     -字符串
 * */
void Hal_oledDisplay_str(uint8_t line, uint8_t column, uint8_t *str);

/* func:
 *      刷新屏幕
 * */
void Hal_oledDisplay_refresh();
void Hal_oledDisplay_init();


#endif /* SOURCE_HAL_HAL_OLEDDISPLAY_H_ */
