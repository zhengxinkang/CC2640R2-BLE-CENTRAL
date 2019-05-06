/*
 * Types.h
 *
 *  Created on: 2017年8月9日
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_COMMON_TYPES_H_
#define SOURCE_COMMON_TYPES_H_

#include <stdint.h>
#include <xdc/std.h>
#include <_hal_types.h>

typedef enum{
    PORT_VALUE_LOW  = 0,
    PORT_VALUE_HIGH = 1
}PortValue;

typedef void (*OnReceived)(uint8_t* data, uint16_t len);

#ifndef TRUE
#define TRUE            1       ///< 布尔值：真
#endif
#ifndef FALSE
#define FALSE           0       ///< 布尔值：假
#endif

#ifndef NULL
#define NULL            ((void *)0)     ///< 空指针
#endif

void delay_step(uint32_t steps);

//#ifndef Int8
//#define Int8                char
//#endif
//
//#ifndef Int16
//#define Int16                short
//#endif

//#ifndef Int32
//#define Int32                long
//#endif
//
//#ifndef Int64
//#define Int64                long long
//#endif

//#ifndef Uint8
//#define Uint8                unsigned char
//#endif
//
//#ifndef Uint16
//#define Uint16                unsigned short
//#endif
//
//#ifndef Uint32
//#define Uint32                unsigned long
//#endif
//
//#ifndef Uint64
//#define Uint64                unsigned long long
//#endif


#endif /* SOURCE_COMMON_TYPES_H_ */
