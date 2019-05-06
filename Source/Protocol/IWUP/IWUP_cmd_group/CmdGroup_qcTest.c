/*
 * CmdGroup_qcTest.c
 *
 *  Created on: 2018年9月13日
 *      Author: zheng
 */

#include "IWUP_config.h"
#include "IWUP_interface.h"
#include "IWUP_cmd.h"

#include "TaskTest.h"
#include "TestProcess_uartEcho.h"
#include "TestProcess_setInfo.h"
#include "TestProcess_self.h"
#include "DataType.h"
#define TRACE_IWUP_CMD          //TRACE_DEBUG
#define TRACE_IWUP_CMD_HEX      //TRACE_DEBUG_HEXGROUP

#if defined     IWUP_ROLE_WIRELESS_MODULE
//0---SendCmd_qcTest_echo---------------------------------------------------------------------------------
void SendCmd_qcTest_echo                    (uint8_t* para, uint8_t paraLen)
{
    Cmd_send(CMD_QCTEST_ECHO, para, paraLen, PORT_1);
}

void RecieveCmdResp_qcTest_echo             (uint8_t* para, uint8_t paraLen)
{
    TRACE_CODE("RecieveCmdResp_qcTest_echo: ");
    TRACE_CODE_HEXGROUP(para, paraLen, ' ');

    if(0 == memcmp(TEST_STRING, para, sizeof(TEST_STRING)))
    {
        TRACE_CODE("qc test echo compare success.\n");
        TestEvent_post(EVENT_TESTPROCESS_UART_ECHO);
    }
}

//1---SendCmd_qcTest_selfTest---------------------------------------------------------------------------------
void SendCmd_qcTest_selfTest                (uint8_t* para, uint8_t paraLen)
{
    Cmd_send(CMD_QCTEST_SELFTEST, para, paraLen, PORT_1);
}

void RecieveCmdResp_qcTest_selfTest         (uint8_t* para, uint8_t paraLen)
{
    TRACE_CODE("RecieveCmdResp_qcTest_selfTest: ");
    TRACE_CODE_HEXGROUP(para, paraLen, ' ');

    memcpy(selfTestResult, para, paraLen);
    TestEvent_post(EVENT_TESTPROCESS_SELF);
}

//2---SendCmd_qcTest_action-----------------------------------------------------------------------------------
void SendCmd_qcTest_action                  (uint8_t* para, uint8_t paraLen)
{
    Cmd_send(CMD_QCTEST_ACTION, para, paraLen, PORT_1);
}

void RecieveCmdResp_qcTest_action           (uint8_t* para, uint8_t paraLen)
{
    TRACE_CODE("RecieveCmdResp_qcTest_action: ");
    TRACE_CODE_HEXGROUP(para, paraLen, ' ');
    TestEvent_post(EVENT_TESTPROCESS_ACTION_ACTIVEBLE);
}

//3---SendCmd_qcTest_getDeviceInfo----------------------------------------------------------------------------
void SendCmd_qcTest_getDeviceInfo           (void)
{
    Cmd_send(CMD_QCTEST_GET_DEVICE_INFO, NULL, 0, PORT_1);
    TRACE_CODE("发送获取设备信息命令。\n");
}

void RecieveCmdResp_qcTest_getDeviceInfo    (uint8_t* para, uint8_t paraLen)
{
    TRACE_CODE("RecieveCmdResp_qcTest_getDeviceInfo: ");
    TRACE_CODE_HEXGROUP(para, paraLen, ' ');

    memcpy(&s_lock_info, para+1, sizeof(s_lock_info));
    TestEvent_post(EVENT_TESTPROCESS_GET_DEVICEINFO);
}

//4---SendCmd_qcTest_getDeviceStatus--------------------------------------------------------------------------
void SendCmd_qcTest_getDeviceStatus         (void)
{
    Cmd_send(CMD_QCTEST_GET_DEVICE_STATUS, NULL, 0, PORT_1);
}

void RecieveCmdResp_qcTest_getDeviceStatus  (uint8_t* para, uint8_t paraLen)
{
    TRACE_CODE("RecieveCmdResp_qcTest_getDeviceStatus: ");
    TRACE_CODE_HEXGROUP(para, paraLen, ' ');
    if(0x00 == para[0])
    {
        memcpy(&s_lock_status, para+1, paraLen-1);
        TestEvent_post(EVENT_TESTPROCESS_GET_DEVICESTATUS);
    }
}

//5---SendCmd_qcTest_setDevicePower---------------------------------------------------------------------------
void SendCmd_qcTest_setDevicePower          (uint8_t* para, uint8_t paraLen)
{
    Cmd_send(CMD_QCTEST_SET_DEVICE_POWER, para, paraLen, PORT_1);
}

void RecieveCmdResp_qcTest_setDevicePower   (uint8_t* para, uint8_t paraLen)
{
    TRACE_CODE("RecieveCmdResp_qcTest_setDevicePower: ");
    TRACE_CODE_HEXGROUP(para, paraLen, ' ');
}

//6---SendCmd_qcTest_setDeviceInfo---------------------------------------------------------------------------
void SendCmd_qcTest_setDeviceInfo          (uint8_t* para, uint8_t paraLen)
{
    Cmd_send(CMD_QCTEST_SET_DEVICE_INFO, para, paraLen, PORT_1);
}

void RecieveCmdResp_qcTest_setDeviceInfo   (uint8_t* para, uint8_t paraLen)
{
    TRACE_CODE("RecieveCmdResp_qcTest_setDeviceInfo: ");
    TRACE_CODE_HEXGROUP(para, paraLen, ' ');
    if(0x00 == para[0])
    {
        TestEvent_post(EVENT_TESTPROCESS_SET_DEVICEINFO);
    }
}

//7---SendCmd_qcTest_getBleInfo---------------------------------------------------------------------------
void SendCmd_qcTest_getBleInfo          (uint8_t* para, uint8_t paraLen)
{
    Cmd_send(CMD_QCTEST_GET_BLE_INFO, para, paraLen, PORT_1);
}

void RecieveCmdResp_qcTest_getBleInfo   (uint8_t* para, uint8_t paraLen)
{
    TRACE_CODE("RecieveCmdResp_qcTest_getBleInfo: ");
    TRACE_CODE_HEXGROUP(para, paraLen, ' ');

    memcpy(&s_ble_info, para, paraLen);
    TestEvent_post(EVENT_TESTPROCESS_GET_BLEINFO);
}
//8---SendCmd_qcTest_writeReadTestResult---------------------------------------------------------------------------
void SendCmd_qcTest_writeReadTestResult          (uint8_t* para, uint8_t paraLen)
{
    Cmd_send(CMD_QCTEST_WRITEREAD_TEST_RESULT, para, paraLen, PORT_1);
}

void RecieveCmdResp_qcTest_writeReadTestResult   (uint8_t* para, uint8_t paraLen)
{
    TRACE_CODE("RecieveCmdResp_qcTest_writeReadTestResult: ");
    TRACE_CODE_HEXGROUP(para, paraLen, ' ');
    if(0x00 == para[0])
    {
        testResult = para[1];
        TestEvent_post(EVENT_TESTPROCESS_WRITEREAD_TESTRESULT);
    }
}

#elif defined   IWUP_ROLE_LOCK_BOARD
//0
void RecieveCmd_qcTest_echo                 (uint8_t* para, uint8_t paraLen)
{
    TRACE_DEBUG("RecieveCmd_qcTest_echo: ");
    TRACE_DEBUG_HEXGROUP(para, paraLen, ' ');
    CmdResp_send(CMD_QCTEST_ECHO, para, paraLen, PORT_1);
}

//1
void RecieveCmd_qcTest_selfTest             (uint8_t* para, uint8_t paraLen)
{
    TRACE_DEBUG("RecieveCmd_qcTest_selfTest: ");
    TRACE_DEBUG_HEXGROUP(para, paraLen, ' ');
    CmdResp_send(CMD_QCTEST_SELFTEST, para, paraLen, PORT_1);
}

//2
void RecieveCmd_qcTest_action               (uint8_t* para, uint8_t paraLen)
{
    TRACE_DEBUG("RecieveCmd_qcTest_action: ");
    TRACE_DEBUG_HEXGROUP(para, paraLen, ' ');
    CmdResp_send(CMD_QCTEST_ACTION, para, paraLen, PORT_1);
}

//3
void RecieveCmd_qcTest_getDeviceInfo        (uint8_t* para, uint8_t paraLen)
{
    TRACE_DEBUG("RecieveCmd_qcTest_getDeviceInfo: ");
    TRACE_DEBUG_HEXGROUP(para, paraLen, ' ');
    CmdResp_send(CMD_QCTEST_GET_DEVICE_INFO, para, paraLen, PORT_1);
}

//4
void RecieveCmd_qcTest_getDeviceStatus      (uint8_t* para, uint8_t paraLen)
{
    TRACE_DEBUG("RecieveCmd_qcTest_getDeviceStatus: ");
    TRACE_DEBUG_HEXGROUP(para, paraLen, ' ');
    CmdResp_send(CMD_QCTEST_GET_DEVICE_STATUS, para, paraLen, PORT_1);
}

//5
void RecieveCmd_qcTest_setDevicePower       (uint8_t* para, uint8_t paraLen)
{
    TRACE_DEBUG("RecieveCmd_qcTest_setDevicePower: ");
    TRACE_DEBUG_HEXGROUP(para, paraLen, ' ');
    CmdResp_send(CMD_QCTEST_SET_DEVICE_POWER, para, paraLen, PORT_1);
}

//6
void RecieveCmd_qcTest_setDeviceInfo        (uint8_t* para, uint8_t paraLen)
{
    TRACE_DEBUG("RecieveCmd_qcTest_setDeviceInfo: ");
    TRACE_DEBUG_HEXGROUP(para, paraLen, ' ');
    CmdResp_send(CMD_QCTEST_SET_DEVICE_INFO, para, paraLen, PORT_1);
}

//7
void RecieveCmd_qcTest_getBleInfo        (uint8_t* para, uint8_t paraLen)
{
    TRACE_DEBUG("RecieveCmd_qcTest_getBleInfo: ");
    TRACE_DEBUG_HEXGROUP(para, paraLen, ' ');
    CmdResp_send(CMD_QCTEST_GET_BLE_INFO, para, paraLen, PORT_1);
}

//8
void RecieveCmd_qcTest_writeReadTestResult        (uint8_t* para, uint8_t paraLen)
{
    TRACE_DEBUG("RecieveCmd_qcTest_writeReadTestResult: ");
    TRACE_DEBUG_HEXGROUP(para, paraLen, ' ');
    CmdResp_send(CMD_QCTEST_WRITEREAD_TEST_RESULT, para, paraLen, PORT_1);
}

#else
    #error "role NOT DEFINE!"
#endif

