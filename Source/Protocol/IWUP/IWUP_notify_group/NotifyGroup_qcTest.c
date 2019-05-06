/*
 * NotifyGroup_qcTest.c
 *
 *  Created on: 2018Äê9ÔÂ13ÈÕ
 *      Author: zheng
 */
#include "IWUP_interface.h"
#include "IWUP_notify.h"
#include "IWUP_config.h"
#include "TaskTest.h"
#include "DataType.h"

#if defined     IWUP_ROLE_WIRELESS_MODULE
#define TRACE_IWUP_CMD          //TRACE_DEBUG
#define TRACE_IWUP_CMD_HEX      //TRACE_DEBUG_HEXGROUP

void RecieveNotify_qcTest_card                  (uint8_t* para, uint8_t paraLen)
{
    TRACE_CODE("RecieveNotify_qcTest_card: ");
    TRACE_CODE_HEXGROUP(para, paraLen, ' ');
    NotifyResp(NOTIFY_QCTEST_CARD, para, paraLen, PORT_1);
    TestEvent_post(EVENT_TESTPROCESS_CARD);
}

void RecieveNotify_qcTest_keyBoard              (uint8_t* para, uint8_t paraLen)
{
    TRACE_CODE("RecieveNotify_qcTest_keyBoard: ");
    TRACE_CODE_HEXGROUP(para, paraLen, ' ');
    NotifyResp(NOTIFY_QCTEST_KEYBOARD, para, paraLen, PORT_1);
    keyValue = (KEY_VALUE)para[0];
    TestEvent_post(EVENT_TESTPROCESS_KEYBOARD);
}

void RecieveNotify_qcTest_event                 (uint8_t* para, uint8_t paraLen)
{
    TRACE_CODE("RecieveNotify_qcTest_event: ");
    TRACE_CODE_HEXGROUP(para, paraLen, ' ');
    NotifyResp(NOTIFY_QCTEST_EVENT, para, paraLen, PORT_1);
    if(para[0] == 0x00)
    {
        TestEvent_post(EVENT_TESTPROCESS_EVENT_SETTINGBUTTON);
    }
    else if(para[0] == 0x01)
    {
        TestEvent_post(EVENT_TESTPROCESS_EVENT_CLEARBUTTON);
    }
    else if(para[0] == 0x02)
    {
        TestEvent_post(EVENT_TESTPROCESS_EVENT_OBLIQUETONGUE);
    }
}

#elif defined   IWUP_ROLE_LOCK_BOARD

//0---SendNotify_qcTest_card-------------------------------------------------------------

void SendNotify_qcTest_card                     (uint8_t* para, uint8_t paraLen)
{
    Notify_send(NOTIFY_QCTEST_CARD, para, paraLen, PORT_1);
}

void RecieveNotifyResp_qcTest_card              (uint8_t* para, uint8_t paraLen)
{
    TRACE_DEBUG("RecieveNotifyResp_qcTest_card: ");
    TRACE_DEBUG_HEXGROUP(para, paraLen, ' ');
}

//1---SendNotify_qcTest_keyBoard-------------------------------------------------------------
void SendNotify_qcTest_keyBoard                   (uint8_t* para, uint8_t paraLen)
{
    Notify_send(NOTIFY_QCTEST_KEYBOARD, para, paraLen, PORT_1);
}

void RecieveNotifyResp_qcTest_keyBoard            (uint8_t* para, uint8_t paraLen)
{
    TRACE_DEBUG("RecieveNotifyResp_qcTest_keyBoard: ");
    TRACE_DEBUG_HEXGROUP(para, paraLen, ' ');
}

//2---SendNotify_qcTest_event-------------------------------------------------------------
void SendNotify_qcTest_event                    (uint8_t* para, uint8_t paraLen)
{
    Notify_send(NOTIFY_QCTEST_EVENT, para, paraLen, PORT_1);
}

void RecieveNotifyResp_qcTest_event             (uint8_t* para, uint8_t paraLen)
{
    TRACE_DEBUG("RecieveNotifyResp_qcTest_event: ");
    TRACE_DEBUG_HEXGROUP(para, paraLen, ' ');
}

#else
#error "role NOT DEFINE!"
#endif
