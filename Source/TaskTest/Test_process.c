/*
 * Test_process.c
 *
 *  Created on: 2018年8月30日
 *      Author: zheng
 */
#include "TaskTest.h"
#include "Trace.h"
#include "Test_process.h"
#include "TestProcess_ble.h"

#define TIMEOUT_TEST_PROCESS_BLE    12000

uint32_t events;

typedef enum{
    TEST_PROCESS_STATE_IDLE     = 0,
    TEST_PROCESS_STATE_START    = 1,
    TEST_PROCESS_STATE_BLE      = 2,
    TEST_PROCESS_STATE_SUCESS   = 3,
    TEST_PROCESS_STATE_FAIL     = 4,
}TEST_PROCESS_STATE;
TEST_PROCESS_STATE test_process_state = TEST_PROCESS_STATE_IDLE;

static uint8_t macAddr[6] = {0x54, 0x6C, 0x0e, 0x9b, 0x58, 0xa4};

RET_TEST_BLE ret_test_ble = RET_TEST_BLE_SUCCESS;

void Test_process()
{
    TEST_PROCESS_STATE test_process_state = TEST_PROCESS_STATE_START;

    test_process_state = TEST_PROCESS_STATE_BLE;

    TestProcess_ble(macAddr);
    events = TestEvent_pend(EVENT_TESTPROCESS_BLE, TIMEOUT_TEST_PROCESS_BLE);
    if (events & EVENT_TESTPROCESS_BLE)
    {
        ret_test_ble = TestProcess_getResult();
        if(RET_TEST_BLE_SUCCESS == ret_test_ble)
        {
            TRACE_ERROR("TestProcess_ble success.", ret_test_ble);
        }
        else
        {

            TRACE_ERROR("TestProcess_ble fail error code = %d!", ret_test_ble);
        }
    }
    else
    {
        TRACE_ERROR("TestProcess_ble time out after %d ms!", TIMEOUT_TEST_PROCESS_BLE);
        test_process_state = TEST_PROCESS_STATE_FAIL;
    }
}

void Test_process_setMacAddr(uint8_t* addr)
{
    memcpy(macAddr, addr, 6);
}
