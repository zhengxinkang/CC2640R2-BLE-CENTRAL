/*
 * CmdGroup_qcTest.h
 *
 *  Created on: 2018Äê9ÔÂ13ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_PROTOCOL_IWUP_IWUP_CMD_GROUP_CMDGROUP_QCTEST_H_
#define SOURCE_PROTOCOL_IWUP_IWUP_CMD_GROUP_CMDGROUP_QCTEST_H_


#include "IWUP_interface.h"
#include "IWUP_config.h"

#if defined     IWUP_ROLE_WIRELESS_MODULE

void SendCmd_qcTest_echo                (uint8_t* para, uint8_t paraLen);
void SendCmd_qcTest_selfTest            (uint8_t* para, uint8_t paraLen);
void SendCmd_qcTest_action              (uint8_t* para, uint8_t paraLen);
void SendCmd_qcTest_getDeviceInfo       (void);
void SendCmd_qcTest_getDeviceStatus     (void);
void SendCmd_qcTest_setDevicePower      (uint8_t* para, uint8_t paraLen);
void SendCmd_qcTest_setDeviceInfo       (uint8_t* para, uint8_t paraLen);
void SendCmd_qcTest_getBleInfo          (uint8_t* para, uint8_t paraLen);
void SendCmd_qcTest_writeReadTestResult (uint8_t* para, uint8_t paraLen);

void RecieveCmdResp_qcTest_echo                 (uint8_t* para, uint8_t paraLen);
void RecieveCmdResp_qcTest_selfTest             (uint8_t* para, uint8_t paraLen);
void RecieveCmdResp_qcTest_action               (uint8_t* para, uint8_t paraLen);
void RecieveCmdResp_qcTest_getDeviceInfo        (uint8_t* para, uint8_t paraLen);
void RecieveCmdResp_qcTest_getDeviceStatus      (uint8_t* para, uint8_t paraLen);
void RecieveCmdResp_qcTest_setDevicePower       (uint8_t* para, uint8_t paraLen);
void RecieveCmdResp_qcTest_setDeviceInfo        (uint8_t* para, uint8_t paraLen);
void RecieveCmdResp_qcTest_getBleInfo           (uint8_t* para, uint8_t paraLen);
void RecieveCmdResp_qcTest_writeReadTestResult  (uint8_t* para, uint8_t paraLen);

#define TABLE_RECIEVE_CMDGROUP_RESP_QCTEST\
    IWUP_CMD_ENTRY(CMD_QCTEST_ECHO,                 RecieveCmdResp_qcTest_echo),\
    IWUP_CMD_ENTRY(CMD_QCTEST_SELFTEST,             RecieveCmdResp_qcTest_selfTest),\
    IWUP_CMD_ENTRY(CMD_QCTEST_ACTION,               RecieveCmdResp_qcTest_action),\
    IWUP_CMD_ENTRY(CMD_QCTEST_GET_DEVICE_INFO,      RecieveCmdResp_qcTest_getDeviceInfo),\
    IWUP_CMD_ENTRY(CMD_QCTEST_GET_DEVICE_STATUS,    RecieveCmdResp_qcTest_getDeviceStatus),\
    IWUP_CMD_ENTRY(CMD_QCTEST_SET_DEVICE_POWER,     RecieveCmdResp_qcTest_setDevicePower),\
    IWUP_CMD_ENTRY(CMD_QCTEST_SET_DEVICE_INFO,      RecieveCmdResp_qcTest_setDeviceInfo),\
    IWUP_CMD_ENTRY(CMD_QCTEST_GET_BLE_INFO,         RecieveCmdResp_qcTest_getBleInfo),\
    IWUP_CMD_ENTRY(CMD_QCTEST_WRITEREAD_TEST_RESULT,RecieveCmdResp_qcTest_writeReadTestResult)

#elif defined   IWUP_ROLE_LOCK_BOARD

void RecieveCmd_qcTest_echo                     (uint8_t* para, uint8_t paraLen);
void RecieveCmd_qcTest_selfTest                 (uint8_t* para, uint8_t paraLen);
void RecieveCmd_qcTest_action                   (uint8_t* para, uint8_t paraLen);
void RecieveCmd_qcTest_getDeviceInfo            (uint8_t* para, uint8_t paraLen);
void RecieveCmd_qcTest_getDeviceStatus          (uint8_t* para, uint8_t paraLen);
void RecieveCmd_qcTest_setDevicePower           (uint8_t* para, uint8_t paraLen);
void RecieveCmd_qcTest_setDeviceInfo            (uint8_t* para, uint8_t paraLen);
void RecieveCmd_qcTest_getBleInfo               (uint8_t* para, uint8_t paraLen);
void RecieveCmd_qcTest_writeReadTestResult      (uint8_t* para, uint8_t paraLen);

#define TABLE_RECIEVE_CMDGROUP_QCTEST\
    IWUP_CMD_ENTRY(CMD_QCTEST_ECHO,                 RecieveCmd_qcTest_echo),\
    IWUP_CMD_ENTRY(CMD_QCTEST_SELFTEST,             RecieveCmd_qcTest_selfTest),\
    IWUP_CMD_ENTRY(CMD_QCTEST_ACTION,               RecieveCmd_qcTest_action),\
    IWUP_CMD_ENTRY(CMD_QCTEST_GET_DEVICE_INFO,      RecieveCmd_qcTest_getDeviceInfo),\
    IWUP_CMD_ENTRY(CMD_QCTEST_GET_DEVICE_STATUS,    RecieveCmd_qcTest_getDeviceStatus),\
    IWUP_CMD_ENTRY(CMD_QCTEST_SET_DEVICE_POWER,     RecieveCmd_qcTest_setDevicePower),\
    IWUP_CMD_ENTRY(CMD_QCTEST_SET_DEVICE_INFO,      RecieveCmd_qcTest_setDeviceInfo),\
    IWUP_CMD_ENTRY(CMD_QCTEST_GET_BLE_INFO,         RecieveCmd_qcTest_getBleInfo),\
    IWUP_CMD_ENTRY(CMD_QCTEST_WRITEREAD_TEST_RESULT,RecieveCmd_qcTest_writeReadTestResult)
#else
    #error "role NOT DEFINE!"
#endif


#endif /* SOURCE_PROTOCOL_IWUP_IWUP_CMD_GROUP_CMDGROUP_QCTEST_H_ */
