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
#include "Trace.h"
#include "BF_Util.h"
#include "DataType.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

#define TIMEOUT_TEST_PROCESS_BLE_DISCOVER   6000
#define TIMEOUT_TEST_PROCESS_BLE_CONNECT    6000
#define TIMEOUT_TEST_PROCESS_DATA_TRANS     1000
#define TIMEOUT_TEST_PROCESS_DISCONNECT     1000
#define TIMEOUT_TEST_PROCESS_GET_BLEINFO    1000
#define TIMEOUT_TEST_PROCESS_ACTION_ACTIVEBLE   1000

void BleTestAction_discover(void);
void BleTestAction_connect(void);
void BleTestAction_dataTransmission(void);
void BleTestAction_writeData(void);
void BleTestAction_readData(void);
void BleTestAction_finish(void);

uint8_t ActionDiscover_Index = 0xff;
uint8_t ActionConnect_Index = 0xff;
uint8_t ActionDataWrite_Index = 0xff;
uint8_t ActionDataRead_Index = 0xff;
uint8_t ActionFinish_Index = 0xff;
bool BleTest_waitUtilDisconnect(bool isSuccessJb);

void TestProcess_bleInit(void)
{
    if(!BF_UtilRegeistItrCallback(BleTestAction_discover, &ActionDiscover_Index))
        TRACE_FATAL("FATAL: ActionDiscover_Index regeist fail!\n");
    if(!BF_UtilRegeistItrCallback(BleTestAction_connect, &ActionConnect_Index))
        TRACE_FATAL("FATAL: ActionConnect_Index regeist fail!\n");
    if(!BF_UtilRegeistItrCallback(BleTestAction_writeData, &ActionDataWrite_Index))
        TRACE_FATAL("FATAL: ActionDataWrite_Index regeist fail!\n");
    if(!BF_UtilRegeistItrCallback(BleTestAction_readData, &ActionDataRead_Index))
        TRACE_FATAL("FATAL: ActionDataRead_Index regeist fail!\n");
    if(!BF_UtilRegeistItrCallback(BleTestAction_finish, &ActionFinish_Index))
        TRACE_FATAL("FATAL: ActionFinish_Index regeist fail!\n");
}

void BleTestAction_discover(void)
{
    RET_DISCOVER ret_discover = BleTest_discover();

    switch(ret_discover)
    {
        case RET_DISCOVER_OK:
        {
            TRACE_CODE("TestProcess_ble discover RET_DISCOVER_OK.\n", ret_discover);
            break;
        }
        case RET_DISCOVER_ERROR_BLE_STATE:
        {
            TRACE_ERROR("TestProcess_ble discover RET_DISCOVER_ERROR_BLE_STATE!\n", ret_discover);
            BleTest_waitUtilDisconnect(FALSE);
            TestEvent_post(EVENT_TESTPROCESS_BLE_DISCOVER_FAIL);
            break;
        }
        case RET_DISCOVER_ERROR_SCAN_RESULT:
        {
            TRACE_ERROR("TestProcess_ble discover RET_DISCOVER_ERROR_SCAN_RESULT!\n", ret_discover);
            TestEvent_post(EVENT_TESTPROCESS_BLE_DISCOVER_FAIL);
            break;
        }
    }
}

void BleTestAction_connect(void)
{
    RET_CONNECT ret_connet = BleTest_connect();

    switch(ret_connet)
    {
        case RET_CONNECT_OK:
        {
            TRACE_CODE("TestProcess_ble connect RET_CONNECT_OK.\n", ret_connet);
            break;
        }
        case RET_CONNECT_ERROR_NODEVICE:
        {
            TRACE_ERROR("TestProcess_ble connect RET_CONNECT_ERROR_NODEVICE!\n", ret_connet);
            break;
        }
        case RET_CONNECT_ERROR_SCANNING:
        {
            TRACE_ERROR("TestProcess_ble connect RET_CONNECT_ERROR_SCANNING!\n", ret_connet);
            break;
        }
    }
}

void BleTestAction_writeData(void)
{
    BleTest_writeData();
}

void BleTestAction_readData(void)
{
    BleTest_readData();
}

static bool isSuccess = TRUE;
static bool isBleInit = TRUE;
void BleTestAction_finish(void)
{
    isBleInit = BleTest_finish(isSuccess);
}

bool BleTest_waitUtilDisconnect(bool isSuccessJb)
{
    bool ret = FALSE;
    uint32_t events;
    isSuccess = isSuccessJb;
    BF_UtilWaitHandle(ActionFinish_Index);
    events = TestEvent_pend(EVENT_TESTPROCESS_BLE_DISCONNECT, TIMEOUT_TEST_PROCESS_DISCONNECT);
    if (events & EVENT_TESTPROCESS_BLE_DISCONNECT)
    {
        TRACE_CODE("断开连接成功.\n");
        ret = isBleInit;
    }
    else
    {
        TRACE_DEBUG("等待断开连接%dms后超时！\n",TIMEOUT_TEST_PROCESS_DISCONNECT);
        ret = FALSE;
    }
    return ret;
}

RET_TEST_BLE TestProcess_ble(uint8_t* searchAddr)
{
    uint32_t events;
    RET_TEST_BLE ret_test_ble = RET_TEST_BLE_SUCCESS;

    //0-1 获取ble的mac地址
    //发送读取ble信息指令----------------------------------
    if(RET_TEST_BLE_SUCCESS == ret_test_ble)
    {
        Task_sleep((1000 * 1000) / Clock_tickPeriod);
        SendCmd_qcTest_getBleInfo(NULL, 0);
        Task_sleep((100 * 1000) / Clock_tickPeriod);
        //等待接收返回消息
        events = TestEvent_pend(EVENT_TESTPROCESS_GET_BLEINFO, TIMEOUT_TEST_PROCESS_GET_BLEINFO);
        if (events & EVENT_TESTPROCESS_GET_BLEINFO)
        {
            TRACE_CODE("获取到ble mac地址：");
            TRACE_CODE_HEXGROUP(s_ble_info.macAddr, 6, ' ');
            ret_test_ble = RET_TEST_BLE_SUCCESS;
        }
        else
        {
            ret_test_ble = RET_TEST_BLE_ERROR_GETBLEINFO;
            TRACE_DEBUG("获取到ble mac地址 %dms后超时！\n", TIMEOUT_TEST_PROCESS_GET_BLEINFO);
        }
    }

    //0-2 激活ble
    //发送激活ble信息指令----------------------------------
    if(RET_TEST_BLE_SUCCESS == ret_test_ble)
    {
        Task_sleep((200 * 1000) / Clock_tickPeriod);
        ACTION_TYPE actionType = ACTION_TYPE_ACTIVEBLE;
        SendCmd_qcTest_action(&actionType, 1);
        Task_sleep((200 * 1000) / Clock_tickPeriod);
        //等待接收返回消息
        events = TestEvent_pend(EVENT_TESTPROCESS_ACTION_ACTIVEBLE, TIMEOUT_TEST_PROCESS_ACTION_ACTIVEBLE);
        if (events & EVENT_TESTPROCESS_ACTION_ACTIVEBLE)
        {
            ret_test_ble = RET_TEST_BLE_SUCCESS;
            TRACE_CODE("唤醒BLE 成功.\n");
        }
        else
        {
            ret_test_ble = RET_TEST_BLE_ERROR_ACTIVEBLE;
            TRACE_DEBUG("唤醒BLE %dms后超时！\n", TIMEOUT_TEST_PROCESS_ACTION_ACTIVEBLE);
        }
    }

    //1-开始发现设备-dicover
    if(RET_TEST_BLE_SUCCESS == ret_test_ble)
    {
        TRACE_CODE("TestProcess_ble dicover start...\n");
        if(NULL == searchAddr)
        {
//            uint8_t addr[6] = {0x00, 0x81, 0xf9, 0x46, 0x07, 0xd6};
            uint8_t addr[6];
            memcpy(addr, s_ble_info.macAddr, 6);
            simpleCentral_setSearchDeviceAddr(addr);
        }
        else
        {
            simpleCentral_setSearchDeviceAddr(searchAddr);
        }
        BF_UtilWaitHandle(ActionDiscover_Index);
        events = TestEvent_pend(EVENT_TESTPROCESS_BLE_DISCOVER|EVENT_TESTPROCESS_BLE_DISCOVER_FAIL, TIMEOUT_TEST_PROCESS_BLE_DISCOVER);
        if (events & EVENT_TESTPROCESS_BLE_DISCOVER)
        {
            TRACE_CODE("TestProcess_ble dicover success.\n");
            ret_test_ble = RET_TEST_BLE_SUCCESS;
        }
        else if(events & EVENT_TESTPROCESS_BLE_DISCOVER_FAIL)
        {
            TRACE_ERROR("TestProcess_ble dicover timeout after %d ms!\n", TIMEOUT_TEST_PROCESS_BLE_DISCOVER);
            ret_test_ble = RET_TEST_BLE_DISCOVERTIMEOUT;
        }
    }

    //2-从列表中搜索设备-findDeviceInList
    if(RET_TEST_BLE_SUCCESS == ret_test_ble)
    {
        TRACE_CODE("TestProcess_ble findDeviceInList start...\n");
        RET_FIND_DEVICE_INLIST ret_findDeviceInList = BleTest_findDeviceInList();
        switch(ret_findDeviceInList)
        {
            case RET_FIND_DEVICE_INLIST_OK:
            {
                TRACE_CODE("TestProcess_ble findDeviceInList: RET_FIND_DEVICE_INLIST_OK.\n");
                ret_test_ble = RET_TEST_BLE_SUCCESS;
                break;
            }
            case RET_FIND_DEVICE_INLIST_ERROR_SCANNING:
            {
                TRACE_DEBUG("TestProcess_ble findDeviceInList: RET_FIND_DEVICE_INLIST_ERROR_SCANNING!\n");
                ret_test_ble = RET_TEST_BLE_NOFIND;
                break;
            }
            case RET_FIND_DEVICE_INLIST_ERROR_NODEVICE:
            {
                TRACE_DEBUG("TestProcess_ble findDeviceInList: RET_FIND_DEVICE_INLIST_ERROR_NODEVICE!\n");
                ret_test_ble = RET_TEST_BLE_NOFIND;
                break;
            }
            case RET_FIND_DEVICE_INLIST_ERROR_NOFIND:
            {
                TRACE_DEBUG("TestProcess_ble findDeviceInList: RET_FIND_DEVICE_INLIST_ERROR_NOFIND!\n");
                ret_test_ble = RET_TEST_BLE_NOFIND;
                break;
            }
        }
    }

    //3-连接设备-connect
    if (RET_TEST_BLE_SUCCESS == ret_test_ble)
    {
        TRACE_CODE("TestProcess_ble connect start...\n");
        Task_sleep(200*(1000/Clock_tickPeriod));
        BF_UtilWaitHandle(ActionConnect_Index);

        events = TestEvent_pend(EVENT_TESTPROCESS_BLE_CONNECT, TIMEOUT_TEST_PROCESS_BLE_CONNECT);
        if (events & EVENT_TESTPROCESS_BLE_CONNECT)
        {
            TRACE_CODE("TestProcess_ble connect success.\n");
            ret_test_ble = RET_TEST_BLE_SUCCESS;
        }
        else
        {
            TRACE_ERROR("TestProcess_ble connect timeout after %d ms!\n", TIMEOUT_TEST_PROCESS_BLE_DISCOVER);
            ret_test_ble = RET_TEST_BLE_ERROR_CONNECT;
            BleTest_waitUtilDisconnect(FALSE);
        }
    }

    //4-数据收发-dataTransmission
    if (RET_TEST_BLE_SUCCESS == ret_test_ble)
    {
        TRACE_CODE("TestProcess_ble dataTransmission start...\n");
        Task_sleep(500*(1000/Clock_tickPeriod));
        for(uint8_t i=0; i<20 ;i++)
        {
            if(0 == i%2)
            {
                BF_UtilWaitHandle(ActionDataWrite_Index);
            }
            else
            {
                BF_UtilWaitHandle(ActionDataRead_Index);
            }

            events = TestEvent_pend(EVENT_TESTPROCESS_BLE_DATA_TRANS, TIMEOUT_TEST_PROCESS_DATA_TRANS);
            if (events & EVENT_TESTPROCESS_BLE_DATA_TRANS)
            {
                if(i == 19&&RET_TEST_BLE_SUCCESS == ret_test_ble)
                {
                    TRACE_CODE("TestProcess_ble dataTransmission success.\n");
                    if(BleTest_waitUtilDisconnect(TRUE))
                    {
                        ret_test_ble = RET_TEST_BLE_SUCCESS;
                    }
                    else
                    {
                        ret_test_ble = RET_TEST_BLE_ERROR_NOTINIT;
                    }
                    break;
                }
            }
            else
            {
                TRACE_ERROR("TestProcess_ble dataTransmission timeout after %d ms!\n", TIMEOUT_TEST_PROCESS_DATA_TRANS);
                ret_test_ble = RET_TEST_BLE_ERROR_DATATRANS;
                BleTest_waitUtilDisconnect(FALSE);
                break;
            }
        }
    }
    return ret_test_ble;
}
