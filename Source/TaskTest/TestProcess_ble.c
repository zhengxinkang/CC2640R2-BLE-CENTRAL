/*
 * TestProcess_ble.c
 *
 *  Created on: 2018年8月30日
 *      Author: zheng
 */
#include "TaskTest.h"
#include "TestProcess_ble.h"
#include "simple_central.h"
#include "MyEventConfig.h"

static RET_TEST_BLE ret_test_ble = RET_TEST_BLE_SUCCESS;

void TestProcess_ble(uint8_t* searchAddr)
{
    simpleCentral_setSearchDeviceAddr(searchAddr);
    MyEvent_post(MY_TEST_PROCESS);
}

extern void MyEvent_testProcessHandle()
{
    simpleCentral_action();
}

RET_TEST_BLE TestProcess_getResult()
{
    return ret_test_ble;
}

void TestProcess_setResult(RET_TEST_BLE ret)
{
    ret_test_ble = ret;
    TestEvent_post(EVENT_TESTPROCESS_BLE);
}
