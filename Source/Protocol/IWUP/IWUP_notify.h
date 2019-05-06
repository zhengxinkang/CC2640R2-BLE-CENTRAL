/*
 * IWUP_notify.h
 *
 *  Created on: 2017Äê12ÔÂ19ÈÕ
 *      Author: Administrator
 */

#ifndef SOURCE_PROTOCOL_IWUP_IWUP_NOTIFY_H_
#define SOURCE_PROTOCOL_IWUP_IWUP_NOTIFY_H_


#include "IWUP_config.h"

#if defined     IWUP_ROLE_WIRELESS_MODULE

void Recieve_notify     (uint8_t cmdGroup, uint8_t cmdCode, uint8_t* para, uint8_t paraLen);
void NotifyResp         (uint16_t cmdGroupCode, uint8_t* para, uint8_t paraLen, uint8_t portNum);

#elif defined   IWUP_ROLE_LOCK_BOARD

void Recieve_notifyResp (uint8_t cmdGroup, uint8_t cmdCode, uint8_t* para, uint8_t paraLen);
void Notify_send        (uint16_t cmdGroupCode, uint8_t* para, uint8_t paraLen, uint8_t portNum);

#else
    #error "role NOT DEFINE!"
#endif


#endif /* SOURCE_PROTOCOL_IWUP_IWUP_NOTIFY_H_ */
