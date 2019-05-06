/*
 * IWUP_config.h
 *
 *  Created on: 2017��12��19��
 *      Author: Administrator
 */

#ifndef SOURCE_PROTOCOL_IWUP_IWUP_CONFIG_H_
#define SOURCE_PROTOCOL_IWUP_IWUP_CONFIG_H_


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Trace.h"
#include "Config/ConfigProtocol.h"
#include "IwupUart_adapter.h"

#define IWUP_VERSION {0x12,0x06}    //Э��汾�ţ�1.2.0.6
//---��ɫѡ��-------------------------------------------------------------------------------------
//#define IWUP_ROLE_WIRELESS_MODULE
//#define IWUP_ROLE_LOCK_BOARD
//---�������-------------------------------------------------------------------------------------
//#define TRACE_ERROR(fmt, ...)
//#define TRACE_ERROR_HEXGROUP(_x,_y,_z)
//
//#define TRACE_DEBUG(fmt, ...)
//#define TRACE_DEBUG_HEXGROUP(_x,_y,_z)

//-----------------------------------------------------------------------------------------------
#if defined     IWUP_ROLE_WIRELESS_MODULE
    #define IWUP_ROLE_STRING "<WIRELESS_MODULE>\n"
#elif defined   IWUP_ROLE_LOCK_BOARD
    #define IWUP_ROLE_STRING "\n<LOCK_BOARD>\n"
#else
    #error "IWUP role NOT DEFINE!"
#endif

#endif /* SOURCE_PROTOCOL_IWUP_IWUP_CONFIG_H_ */
