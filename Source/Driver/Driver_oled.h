/*
 * Driver_oled.h
 *
 *  Created on: 2018骞�11鏈�24鏃�
 *      Author: zheng
 */

#ifndef SOURCE_DRIVER_DRIVER_OLED_H_
#define SOURCE_DRIVER_DRIVER_OLED_H_


//////////////////////////////////////////////////////////////////////////////////
//閺堫剛鈻兼惔蹇撳涧娓氭稑顒熸稊鐘卞▏閻㈩煉绱濋張顏嗙病娴ｆ粏锟藉懓顔忛崣顖ょ礉娑撳秴绶遍悽銊ょ艾閸忚泛鐣犳禒璁崇秿閻€劑锟斤拷
//娑擃厽娅欓崶顓犳暩鐎涳拷
//鎼存鎽甸崷鏉挎絻閿涙ttp://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  閺傦拷 娴狅拷 閸氾拷   : main.c
//  閻楋拷 閺堬拷 閸欙拷   : v2.0
//  娴ｏ拷    閼帮拷   : HuangKai
//  閻㈢喐鍨氶弮銉︽埂   : 2014-0101
//  閺堬拷鏉╂垳鎱ㄩ弨锟�   :
//  閸旂喕鍏橀幓蹇氬牚   : OLED 4閹恒儱褰涘鏃傘仛娓氬鈻�(51缁鍨�)
//              鐠囧瓨妲�:
//              ----------------------------------------------------------------
//              GND    閻㈠灚绨崷锟�
//              VCC  閹猴拷5V閹达拷3.3v閻㈠灚绨�
//              D0   閹侯櫀A5閿涘湯CL閿涳拷
//              D1   閹侯櫀A7閿涘湯DA閿涳拷
//              RES  閹侯櫀B0
//              DC   閹侯櫀B1
//              CS   閹侯櫀A4
//              ----------------------------------------------------------------
// 娣囶喗鏁奸崢鍡楀蕉   :
// 閺冿拷    閺堬拷   :
// 娴ｏ拷    閼帮拷   : HuangKai
// 娣囶喗鏁奸崘鍛啇   : 閸掓稑缂撻弬鍥︽
//閻楀牊娼堥幍锟介張澶涚礉閻╂澧楄箛鍛敀閵嗭拷
//Copyright(C) 娑擃厽娅欓崶顓犳暩鐎涳拷2014/3/16
//All rights reserved
//******************************************************************************/
//#include "sys.h"
#include "stdlib.h"
#include "Driver_gpio.h"

#define u32 uint32_t
#define u8 uint8_t


//OLED濡�崇础鐠佸墽鐤�
//0:4缁惧じ瑕嗙悰灞灸佸锟�
//1:楠炴儼顢�8080濡�崇础
#define OLED_MODE 0
#define SIZE 16
#define XLevelL     0x00
#define XLevelH     0x10
#define Max_Column  128
#define Max_Row     64
#define Brightness  0xFF
#define X_WIDTH     128
#define Y_WIDTH     64

//-----------------濞村鐦疞ED缁旑垰褰涚�规矮绠�----------------
#define LED_ON GPIO_ResetBits(GPIOB,GPIO_Pin_8)//DC
#define LED_OFF GPIO_SetBits(GPIOB,GPIO_Pin_8)



//-----------------OLED缁旑垰褰涚�规矮绠�----------------
#define OLED_SCLK_Clr() Driver_gpioSet(PORT_OLED_SCLK, PORT_VALUE_LOW)  //GPIO_ResetBits(GPIOA,GPIO_Pin_5)//CLK
#define OLED_SCLK_Set() Driver_gpioSet(PORT_OLED_SCLK, PORT_VALUE_HIGH) //GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_SDIN_Clr() Driver_gpioSet(PORT_OLED_SDIN, PORT_VALUE_LOW)  //GPIO_ResetBits(GPIOA,GPIO_Pin_7)//DIN
#define OLED_SDIN_Set() Driver_gpioSet(PORT_OLED_SDIN, PORT_VALUE_HIGH) //GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define OLED_RST_Clr()  Driver_gpioSet(PORT_OLED_RST, PORT_VALUE_LOW)  //GPIO_ResetBits(GPIOB,GPIO_Pin_0)//RES
#define OLED_RST_Set()  Driver_gpioSet(PORT_OLED_RST, PORT_VALUE_HIGH) //GPIO_SetBits(GPIOB,GPIO_Pin_0)

#define OLED_DC_Clr()   Driver_gpioSet(PORT_OLED_DC, PORT_VALUE_LOW)  //GPIO_ResetBits(GPIOB,GPIO_Pin_1)//DC
#define OLED_DC_Set()   Driver_gpioSet(PORT_OLED_DC, PORT_VALUE_HIGH) //GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define OLED_CS_Clr()   Driver_gpioSet(PORT_OLED_CS, PORT_VALUE_LOW)  //GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CS
#define OLED_CS_Set()   Driver_gpioSet(PORT_OLED_CS, PORT_VALUE_HIGH) //GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define OLED_CMD  0 //閸愭瑥鎳℃禒锟�
#define OLED_DATA 1 //閸愭瑦鏆熼幑锟�


//OLED閹貉冨煑閻€劌鍤遍弫锟�
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len);
void OLED_ShowString(u8 x,u8 y, u8 *p);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
u32 oled_pow(u8 m, u8 n);

#endif /* SOURCE_DRIVER_DRIVER_OLED_H_ */
