/*
 * Hal_keyBoardLed.h
 *
 *  Created on: 2018Äê12ÔÂ6ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_HAL_HAL_KEYBOARDLED_H_
#define SOURCE_HAL_HAL_KEYBOARDLED_H_


//µÆ ÐòºÅ        0          1          2          3          4          5          6          7          8         9          10          11
//µÆ code    16(R_LED1) 17(R_LED2) 18(R_LED3) 19(R_LED4) 20(R_LED5) 21(R_LED6) 22(R_LED7) 23(R_LED8) 8(R_LED9) 9(R_LED10) 10(R_LED11) 11(R_LED12)
//µÆ char    '1'        '2'        '3'        '4'        '5'        '6'        '7'        '8'        '9'       '*'        '0'         '#'
char                LedSerialToChar(uint8_t serial);
EXPAND_INPUT_SERIAL LedSerialToCode(uint8_t serial);


#endif /* SOURCE_HAL_HAL_KEYBOARDLED_H_ */
