/*
 * Hal_oledDisplay.h
 *
 *  Created on: 2019��5��11��
 *      Author: zheng
 */

#ifndef SOURCE_HAL_HAL_OLEDDISPLAY_H_
#define SOURCE_HAL_HAL_OLEDDISPLAY_H_

/* func:
 *      ��ʾ����
 * para:
 *      line    -��
 *      column  -ƫ��
 *      num     -����
 *      len     -����
 * */
void Hal_oledDisplay_num(uint8_t line, uint8_t column, uint32_t num, uint8_t len);

/* func:
 *      ��ʾ�ַ���
 * para:
 *      line    -��
 *      column  -ƫ��
 *      str     -�ַ���
 * */
void Hal_oledDisplay_str(uint8_t line, uint8_t column, uint8_t *str);

/* func:
 *      ˢ����Ļ
 * */
void Hal_oledDisplay_refresh();
void Hal_oledDisplay_init();


#endif /* SOURCE_HAL_HAL_OLEDDISPLAY_H_ */
