/*
 * TestProcess_ble.h
 *
 *  Created on: 2018年8月30日
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_BLE_H_
#define SOURCE_TASKTEST_TESTPROCESS_BLE_H_


typedef enum{
    RET_TEST_BLE_SUCCESS                    = 0,
    RET_TEST_BLE_TIMEOUT                    = 1,
    RET_TEST_BLE_DISCOVERTIMEOUT            = 2,
    RET_TEST_BLE_NOFIND                     = 3,
    RET_TEST_BLE_ERROR_MTUSIZE              = 4,
    RET_TEST_BLE_ERROR_SERVICE              = 5,
    RET_TEST_BLE_ERROR_PROCEDURE_COMPLETE   = 6,
    RET_TEST_BLE_ERROR_CHARACTERISTIC       = 7,
    RET_TEST_BLE_ERROR_CONNECT              = 8,
    RET_TEST_BLE_ERROR_DATATRANS            = 9,
    RET_TEST_BLE_ERROR_GETBLEINFO           = 10,
    RET_TEST_BLE_ERROR_ACTIVEBLE            = 11,
    RET_TEST_BLE_ERROR_NOTINIT              = 12,   //蓝牙模块没有初始化
}RET_TEST_BLE;

void TestProcess_bleInit(void);
RET_TEST_BLE TestProcess_ble(uint8_t* searchAddr);
//RET_TEST_BLE TestProcess_getResult();
void TestProcess_setResult(RET_TEST_BLE ret);
extern void MyEvent_testProcessHandle();

#endif /* SOURCE_TASKTEST_TESTPROCESS_BLE_H_ */
