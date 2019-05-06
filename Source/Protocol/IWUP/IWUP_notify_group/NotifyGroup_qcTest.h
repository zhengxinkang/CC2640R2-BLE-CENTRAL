/*
 * NotifyGroup_qcTest.h
 *
 *  Created on: 2018Äê9ÔÂ13ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_PROTOCOL_IWUP_IWUP_NOTIFY_GROUP_NOTIFYGROUP_QCTEST_H_
#define SOURCE_PROTOCOL_IWUP_IWUP_NOTIFY_GROUP_NOTIFYGROUP_QCTEST_H_


#include "IWUP_interface.h"
#include "IWUP_config.h"
#if defined     IWUP_ROLE_WIRELESS_MODULE

void RecieveNotify_qcTest_card                  (uint8_t* para, uint8_t paraLen);
void RecieveNotify_qcTest_keyBoard              (uint8_t* para, uint8_t paraLen);
void RecieveNotify_qcTest_event                 (uint8_t* para, uint8_t paraLen);

#define TABLE_RECIEVE_NOTIFYGROUP_QCTEST\
    IWUP_CMD_ENTRY(NOTIFY_QCTEST_CARD,              RecieveNotify_qcTest_card),\
    IWUP_CMD_ENTRY(NOTIFY_QCTEST_KEYBOARD,          RecieveNotify_qcTest_keyBoard),\
    IWUP_CMD_ENTRY(NOTIFY_QCTEST_EVENT,             RecieveNotify_qcTest_event)

#elif defined   IWUP_ROLE_LOCK_BOARD

void SendNotify_qcTest_card                     (uint8_t* para, uint8_t paraLen);
void SendNotify_qcTest_keyBoard                 (uint8_t* para, uint8_t paraLen);
void SendNotify_qcTest_event                    (uint8_t* para, uint8_t paraLen);

void RecieveNotifyResp_qcTest_card                      (uint8_t* para, uint8_t paraLen);
void RecieveNotifyResp_qcTest_keyBoard                  (uint8_t* para, uint8_t paraLen);
void RecieveNotifyResp_qcTest_event                     (uint8_t* para, uint8_t paraLen);

#define TABLE_RECIEVE_NOTIFYGROUP_RESP_QCTEST\
    IWUP_CMD_ENTRY(NOTIFY_QCTEST_CARD,                  RecieveNotifyResp_qcTest_card),\
    IWUP_CMD_ENTRY(NOTIFY_QCTEST_KEYBOARD,              RecieveNotifyResp_qcTest_keyBoard),\
    IWUP_CMD_ENTRY(NOTIFY_QCTEST_EVENT,                 RecieveNotifyResp_qcTest_event)

#else
#error "role NOT DEFINE!"
#endif


#endif /* SOURCE_PROTOCOL_IWUP_IWUP_NOTIFY_GROUP_NOTIFYGROUP_QCTEST_H_ */
