/*
 * TestProcess_ble.h
 *
 *  Created on: 2018Äê8ÔÂ30ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TESTPROCESS_BLE_H_
#define SOURCE_TASKTEST_TESTPROCESS_BLE_H_


typedef enum{
    RET_TEST_BLE_SUCCESS                    = 0,
    RET_TEST_BLE_TIMEOUT                    = 1,
    RET_TEST_BLE_NOFIND                     = 2,
    RET_TEST_BLE_ERROR_MTUSIZE              = 3,
    RET_TEST_BLE_ERROR_SERVICE              = 4,
    RET_TEST_BLE_ERROR_PROCEDURE_COMPLETE   = 5,
    RET_TEST_BLE_ERROR_CHARACTERISTIC       = 6,
}RET_TEST_BLE;

void TestProcess_ble(uint8_t* searchAddr);
RET_TEST_BLE TestProcess_getResult();
void TestProcess_setResult(RET_TEST_BLE ret);
extern void MyEvent_testProcessHandle();

#endif /* SOURCE_TASKTEST_TESTPROCESS_BLE_H_ */
