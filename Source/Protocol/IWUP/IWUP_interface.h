/*
 * IWUP_interface.h
 *
 *  Created on: 2017年12月19日
 *      Author: Administrator
 */

#ifndef SOURCE_PROTOCOL_IWUP_IWUP_CMD_GROUP_IWUP_INTERFACE_H_
#define SOURCE_PROTOCOL_IWUP_IWUP_CMD_GROUP_IWUP_INTERFACE_H_


#include "IWUP_config.h"

/*****************************************************************************************
 ******************************************命令组******************************************
 *****************************************************************************************/
//---命令组：一般命令CMDGROUP_GENERAL                       【0x00】---------------------------------------------
#define CMDGROUP_GENERAL                                    (0x00<<8)
#define CMD_GENERAL_ECHO                                    (CMDGROUP_GENERAL + 0x00)
#define CMD_GENERAL_APP_CONFIRM                             (CMDGROUP_GENERAL + 0x01)
#define CMD_GENERAL_GET_DEVICE_INFO                         (CMDGROUP_GENERAL + 0x02)
#define CMD_GENERAL_GET_DEVICE_STATUS                       (CMDGROUP_GENERAL + 0x03)
#define CMD_GENERAL_CHANGE_USESTATUS                        (CMDGROUP_GENERAL + 0x04)
#define CMD_GENERAL_SET_DEVICEINFO                          (CMDGROUP_GENERAL + 0x05)

//---命令组：钥匙管理CMDGROUP_KEY_MANAGE                    【0x01】---------------------------------------------
#define CMDGROUP_KEY_MANAGE                                 (0x01<<8)
#define CMD_KEY_MANAGE_ADD_PASSWORD                         (CMDGROUP_KEY_MANAGE + 0x00)
#define CMD_KEY_MANAGE_ENTERORQUIT_ADD_FINGERPRINT_MODE     (CMDGROUP_KEY_MANAGE + 0x01)
#define CMD_KEY_MANAGE_ENTERORQUIT_ADD_CARD_MODE            (CMDGROUP_KEY_MANAGE + 0x02)
#define CMD_KEY_MANAGE_DEL                                  (CMDGROUP_KEY_MANAGE + 0x03)
#define CMD_KEY_MANAGE_GET_COUNT                            (CMDGROUP_KEY_MANAGE + 0x04)
#define CMD_KEY_MANAGE_GET_DETAILS                          (CMDGROUP_KEY_MANAGE + 0x05)
#define CMD_KEY_MANAGE_RESTORE_FACTORY                      (CMDGROUP_KEY_MANAGE + 0x06)
#define CMD_KEY_MANAGE_CHANGE_PASSWORD                      (CMDGROUP_KEY_MANAGE + 0x07)

//---命令组：设备动作管理CMDGROUP_ACTION                    【0x02】---------------------------------------------
#define CMDGROUP_ACTION                                     (0x02<<8)
#define CMD_ACTION_UNLOCK                                   (CMDGROUP_ACTION + 0x00)
#define CMD_ACTION_ALWAYS_OPEN                              (CMDGROUP_ACTION + 0x01)
#define CMD_ACTION_PHONE_ANTILOCK                           (CMDGROUP_ACTION + 0x02)
#define CMD_ACTION_SET_UNLOCKBEEP                           (CMDGROUP_ACTION + 0x03)
#define CMD_ACTION_PREVENT_DISMANTLE                        (CMDGROUP_ACTION + 0x04)
#define CMD_ACTION_BLE_STATE_CHANGE                         (CMDGROUP_ACTION + 0x05)

//---命令组：设备质量检测CMDGROUP_QCTEST                    【0x70】---------------------------------------------
#define CMDGROUP_QCTEST                                     (0x70<<8)
#define CMD_QCTEST_ECHO                                     (CMDGROUP_QCTEST + 0x00)
#define CMD_QCTEST_SELFTEST                                 (CMDGROUP_QCTEST + 0x01)
#define CMD_QCTEST_ACTION                                   (CMDGROUP_QCTEST + 0x02)
#define CMD_QCTEST_GET_DEVICE_INFO                          (CMDGROUP_QCTEST + 0x03)
#define CMD_QCTEST_GET_DEVICE_STATUS                        (CMDGROUP_QCTEST + 0x04)
#define CMD_QCTEST_SET_DEVICE_POWER                         (CMDGROUP_QCTEST + 0x05)
#define CMD_QCTEST_SET_DEVICE_INFO                          (CMDGROUP_QCTEST + 0x06)
#define CMD_QCTEST_GET_BLE_INFO                             (CMDGROUP_QCTEST + 0x07)
#define CMD_QCTEST_WRITEREAD_TEST_RESULT                    (CMDGROUP_QCTEST + 0x08)

/*****************************************************************************************
 ******************************************通知组******************************************
 *****************************************************************************************/
//---通知组：一般通知NOTIFYGROUP_GENERAL                    【0x80】---------------------------------------------
#define NOTIFYGROUP_GENERAL                                 (0x80<<8)
#define NOTIFY_GENERAL_ECHO                                 (NOTIFYGROUP_GENERAL + 0x00)
#define NOTIFY_GENERAL_GET_ID                               (NOTIFYGROUP_GENERAL + 0x01)
#define NOTIFY_GENERAL_ENABLE_ADDWIRELESS                   (NOTIFYGROUP_GENERAL + 0x02)
#define NOTIFY_GENERAL_ADD_FINGERPRINT                      (NOTIFYGROUP_GENERAL + 0x03)
#define NOTIFY_GENERAL_ADD_CARD                             (NOTIFYGROUP_GENERAL + 0x04)
#define NOTIFY_GENERAL_RESTORE_FACTORY                      (NOTIFYGROUP_GENERAL + 0x05)
#define NOTIFY_GENERAL_LOCK_WAKE                            (NOTIFYGROUP_GENERAL + 0x06)

//---通知组：设备质量检测通知NOTIFYGROUP_QCTEST             【0xF0】---------------------------------------------
#define NOTIFYGROUP_QCTEST                                  (0xF0<<8)
#define NOTIFY_QCTEST_CARD                                  (NOTIFYGROUP_QCTEST + 0x00)
#define NOTIFY_QCTEST_KEYBOARD                              (NOTIFYGROUP_QCTEST + 0x01)
#define NOTIFY_QCTEST_EVENT                                 (NOTIFYGROUP_QCTEST + 0x02)


#endif /* SOURCE_PROTOCOL_IWUP_IWUP_CMD_GROUP_IWUP_INTERFACE_H_ */
