///*
// * Driver_voice.c
// *
// *  Created on: 2018年12月3日
// *      Author: zheng
// */
//
//#include "Driver_voice.h"
//#include "stdio.h"
//#include "string.h"
//#include "Driver_gpio.h"
//
//void delay1ms(unsigned char count) //1MS 延时子程序
//{
//    unsigned char i, j, k;
//    for (k = count; k > 0; k--)
//        for (i = 2; i > 0; i--)
//            for (j = 248; j > 0; j--)
//                ;
//}
//void delay100us(void) //100US 延时子程序
//{
//    unsigned char j;
//    for (j = 50; j > 0; j--)
//        ;
//}
//
//void Send_threelines(unsigned char addr) //三线发码子程序
//{
//    unsigned char i;
////    RST = 0;
////    delay1ms(5);
////    RST = 1;
////    delay1ms(17); /* 17ms*/
////    CS = 0;
//    CLR_CS_3A;
//    delay1ms(2);
//    for (i = 0; i < 8; i++)
//    {
////        SCL = 0;
//        CLR_CLK_3A;
//        if (addr & 1)
////            SDA = 1;
//            SET_DATA_3A;
//        else
////            SDA = 0;
//            CLR_DATA_3A;
//        addr >>= 1;
//        delay100us(); /* 100us */
////        SCL = 1;
//        SET_CLK_3A;
//        delay100us();
//    }
////    CS = 1;
//    SET_CS_3A;
//}
