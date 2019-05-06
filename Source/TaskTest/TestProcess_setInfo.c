/*
 * TestProcess_setInfo.c
 *
 *  Created on: 2018��9��27��
 *      Author: zheng
 */

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TestProcess_uartEcho.h"
#include "TestProcess_setInfo.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "Config_TargetBord.h"
#include "DataType.h"
#include "BF_Util.h"
#include "TestProcess_resetTarget.h"


#define TIMEOUT_TEST_PROCESS_SET_DEVICEINFO     1000
#define TIMEOUT_TEST_PROCESS_GET_DEVICEINFO     800
#define MAX_TIME_SETINFO_TRY                    2

RET_TEST_SETINFO TestProcess_setInfo(TARGET_MODEL targetModel)
{
    RET_TEST_SETINFO ret;
    //����������Ϣָ��----------------------------------

    switch(targetModel)
    {
        case TARGET_MODEL_ERROR:
        {
            TRACE_FATAL("�������󣺲��԰����ô���δ֪������!\n");
        }
        case TARGET_MODEL_S31:
        {
            s_setDeviceInfo.factory         = TARGET_BOARD_DEVICE_FACTORY;
            s_setDeviceInfo.model           = TARGET_BOARD_DEVICE_MODEL_S31;
            s_setDeviceInfo.funcAttribute   = TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_S31;
            s_setDeviceInfo.gatewayType     = TARGET_BOARD_DEVICE_GATEWAY_TYPE_S31;
            break;
        }
        case TARGET_MODEL_S31X:
        {
            s_setDeviceInfo.factory         = TARGET_BOARD_DEVICE_FACTORY;
            s_setDeviceInfo.model           = TARGET_BOARD_DEVICE_MODEL_S31X;
            s_setDeviceInfo.funcAttribute   = TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_S31X;
            s_setDeviceInfo.gatewayType     = TARGET_BOARD_DEVICE_GATEWAY_TYPE_S31X;
            break;
        }
        case TARGET_MODEL_D3111:
        {
            s_setDeviceInfo.factory         = TARGET_BOARD_DEVICE_FACTORY;
            s_setDeviceInfo.model           = TARGET_BOARD_DEVICE_MODEL_D3111;
            s_setDeviceInfo.funcAttribute   = TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_D3111;
            s_setDeviceInfo.gatewayType     = TARGET_BOARD_DEVICE_GATEWAY_TYPE_D3111;
            break;
        }
        case TARGET_MODEL_D3151:
        {
            s_setDeviceInfo.factory         = TARGET_BOARD_DEVICE_FACTORY;
            s_setDeviceInfo.model           = TARGET_BOARD_DEVICE_MODEL_D3151;
            s_setDeviceInfo.funcAttribute   = TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_D3151;
            s_setDeviceInfo.gatewayType     = TARGET_BOARD_DEVICE_GATEWAY_TYPE_D3151;
            break;
        }
        case TARGET_MODEL_D3100:
        {
            s_setDeviceInfo.factory         = TARGET_BOARD_DEVICE_FACTORY;
            s_setDeviceInfo.model           = TARGET_BOARD_DEVICE_MODEL_D3100;
            s_setDeviceInfo.funcAttribute   = TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_D3100;
            s_setDeviceInfo.gatewayType     = TARGET_BOARD_DEVICE_GATEWAY_TYPE_D3100;
            break;
        }
    }
    BF_taskSleepMs(500);

    for(uint8_t time=0; time<MAX_TIME_SETINFO_TRY; time++)
    {
        SendCmd_qcTest_setDeviceInfo((uint8_t*)&s_setDeviceInfo, sizeof(s_setDeviceInfo));

        //�ȴ����շ�����Ϣ
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_SET_DEVICEINFO, TIMEOUT_TEST_PROCESS_SET_DEVICEINFO);
        if (events & EVENT_TESTPROCESS_SET_DEVICEINFO)
        {
            ret = RET_TEST_SETINFO_SUCCESS;
            TRACE_CODE("�����豸��Ϣ�ɹ�.\n");
            break;
        }
        else
        {
            ret = RET_TEST_SETINFO_ERROR;
            TRACE_CODE("�����豸��Ϣ%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_SET_DEVICEINFO);
            TestProcess_resetTarget();
        }
    }
    return ret;
}

RET_TEST_SETINFO TestProcess_readInfo(TARGET_MODEL targetModel)
{
    RET_TEST_SETINFO ret;
    //���Ͷ�ȡ��Ϣָ��----------------------------------
    Task_sleep((200*1000)/Clock_tickPeriod);
    SendCmd_qcTest_getDeviceInfo();

    //�ȴ����շ�����Ϣ
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_GET_DEVICEINFO, TIMEOUT_TEST_PROCESS_GET_DEVICEINFO);
    if (events & EVENT_TESTPROCESS_GET_DEVICEINFO)
    {
        if(
            s_lock_info.factory         ==  s_setDeviceInfo.factory&&
            s_lock_info.model           ==  s_setDeviceInfo.model&&
            s_lock_info.funcAttribute   ==  s_setDeviceInfo.funcAttribute&&
            s_lock_info.gatewayType     ==  s_setDeviceInfo.gatewayType
        )
        {
            ret = RET_TEST_SETINFO_SUCCESS;
            TRACE_CODE("��ȡ�豸��Ϣ���ҶԱȳɹ�.\n");
        }
        else
        {
            ret = RET_TEST_SETINFO_ERROR;
            TRACE_CODE("��ȡ�豸��Ϣ���Ա�ʧ�ܣ�\n");
        }
    }
    else
    {
        ret = RET_TEST_SETINFO_ERROR;
        TRACE_CODE("��ȡ�豸��Ϣ%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_GET_DEVICEINFO);
    }
    return ret;
}
