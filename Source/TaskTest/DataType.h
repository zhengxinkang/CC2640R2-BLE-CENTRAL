/*
 * DataType.h
 *
 *  Created on: 2018Äê9ÔÂ29ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_DATATYPE_H_
#define SOURCE_TASKTEST_DATATYPE_H_


#include <stdint.h>

typedef enum{
    TEST_RESULT_SUCCESS = 0x80,
    TEST_RESULT_EMPTY = 0xff,
}TEST_RESULT;

typedef enum{
    KEY_VALUE_0 = 0,
    KEY_VALUE_1 = 1,
    KEY_VALUE_2 = 2,
    KEY_VALUE_3 = 3,
    KEY_VALUE_4 = 4,
    KEY_VALUE_5 = 5,
    KEY_VALUE_6 = 6,
    KEY_VALUE_7 = 7,
    KEY_VALUE_8 = 8,
    KEY_VALUE_9 = 9,
    KEY_VALUE_J = 10,
    KEY_VALUE_X = 11,
}KEY_VALUE;

typedef enum{
    ACTION_TYPE_MOTOR_FORWARD   = 0,
    ACTION_TYPE_MOTOR_REVERSE   = 1,
    ACTION_TYPE_VOICE           = 2,
    ACTION_TYPE_LED             = 3,
    ACTION_TYPE_ACTIVEBLE       = 4,
}ACTION_TYPE;

typedef enum{
    POWER_MODE_SLEEP        = 0,
    POWER_MODE_WAKE_ALL     = 1,
    POWER_MODE_NORMAL       = 2,
}POWER_MODE;

typedef struct{
    uint8_t factory;
    uint8_t model;
    uint8_t funcAttribute;
    uint8_t gatewayType;
}S_SET_DEVICE_INFO;

typedef struct{
    uint8_t factory;
    uint8_t model;
    uint8_t hardwareVersion[2];
    uint8_t softwareVersion[2];
    uint8_t funcAttribute;
    uint8_t protocolVersion[2];
    uint8_t gatewayType;
    uint8_t encryptionType;
}S_LOCK_INFO;

typedef struct
{
    unsigned char mechanicalAntiLock    : 1;
    unsigned char electronicLock        : 1;
    unsigned char normallyOpen          : 1;
    unsigned char preventDismantleSwitch: 1;
    unsigned char preventDismantleState : 1;
    unsigned char reserved1             : 1;
    unsigned char reserved2             : 1;
    unsigned char reserved3             : 1;
} SWITCH_STATUS;

typedef struct{
    uint8_t pwoerPercent;
    SWITCH_STATUS switchStatus;
    uint8_t voice;
    uint8_t useState;
}S_LOCK_STATUS;

typedef struct{
    uint8_t uniqueId[8];
    uint8_t deviceHardwareId[4];
    uint8_t factoryKey[16];
    uint8_t userKey[16];
    uint8_t macAddr[6];
}S_BLE_INFO;

typedef enum{
    WRITEREAD_TESTRESULT_TYPE_WRITE = 0x00,
    WRITEREAD_TESTRESULT_TYPE_READ  = 0x01,
    WRITEREAD_TESTRESULT_TYPE_CLEAR  = 0x02,
}WRITEREAD_TESTRESULT_TYPE;

#define LEN_SELF_TEST_RESULT    7

extern S_LOCK_INFO          s_lock_info;
extern S_LOCK_STATUS        s_lock_status;
extern S_SET_DEVICE_INFO    s_setDeviceInfo;
extern KEY_VALUE            keyValue;
extern uint8_t selfTestResult[20];
extern S_BLE_INFO           s_ble_info;
extern uint8_t              testResult;

#endif /* SOURCE_TASKTEST_DATATYPE_H_ */
