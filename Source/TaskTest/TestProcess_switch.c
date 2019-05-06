/*
 * TestProcess_switch.c
 *
 *  Created on: 2018��9��13��
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "Driver_gpio.h"
#include "TaskTest.h"
#include "DataType.h"
#include "Trace.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "TestProcess_switch.h"
#include "Driver_gpio.h"

#define TIMEOUT_TEST_GET_DEVICESTATUS   500
#define TIMEOUT_TEST_GET_DEVICESTATUS   500
#define TIMEOUT_TEST_GET_EVENT          1000

RET_TEST_SWITCH TestProcess_antiLock()
{
    RET_TEST_SWITCH ret = RET_TEST_SWITCH_SUCCESS;

    for(uint8_t i=0; i<4; i++)
    {
        bool dir = i%2;
        //��������ת��----------------------------------
        Driver_gpioSet(PORT_ANTI_LOCK, (PortValue)dir);
        Task_sleep((50*1000)/Clock_tickPeriod);
        //��ȡ��״̬
        SendCmd_qcTest_getDeviceStatus();

        //�ȴ����շ�����Ϣ
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_GET_DEVICESTATUS, TIMEOUT_TEST_GET_DEVICESTATUS);
        if (events & EVENT_TESTPROCESS_GET_DEVICESTATUS)
        {
            TRACE_CODE("��ȡ�豸״̬�ɹ�.\n");
            if(dir == s_lock_status.switchStatus.mechanicalAntiLock)
            {
                TRACE_CODE("�豸״̬�ȶ���ȷ.\n");
                ret = RET_TEST_SWITCH_SUCCESS;
            }
            else
            {
                TRACE_CODE("�豸״̬�ȶ�ʧ�ܣ�\n");
                ret = RET_TEST_SWITCH_ERROR;
                break;
            }
        }
        else
        {
            ret = RET_TEST_SWITCH_ERROR;
            TRACE_CODE("��ȡ�豸״̬%dms��ʱ��\n",TIMEOUT_TEST_GET_DEVICESTATUS);
            break;
        }
    }
    return ret;
}

RET_TEST_SWITCH TestProcess_preventDismantle()
{
    RET_TEST_SWITCH ret = RET_TEST_SWITCH_SUCCESS;

    for(uint8_t i=0; i<4; i++)
    {
        bool dir = i%2;
        //��������ת��----------------------------------
        Driver_gpioSet(PORT_PREVENT_DISMANTLE, (PortValue)dir);
        Task_sleep((50*1000)/Clock_tickPeriod);
        //��ȡ��״̬
        SendCmd_qcTest_getDeviceStatus();

        //�ȴ����շ�����Ϣ
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_GET_DEVICESTATUS, TIMEOUT_TEST_GET_DEVICESTATUS);
        if (events & EVENT_TESTPROCESS_GET_DEVICESTATUS)
        {
            TRACE_CODE("��ȡ�豸״̬�ɹ�.\n");
            if(!dir == s_lock_status.switchStatus.preventDismantleState)
            {
                TRACE_CODE("���˿���״̬�ȶ���ȷ.\n");
                ret = RET_TEST_SWITCH_SUCCESS;
            }
            else
            {
                TRACE_CODE("���˿���״̬�ȶ�ʧ�ܣ�\n");
                ret = RET_TEST_SWITCH_ERROR;
                break;
            }
        }
        else
        {
            ret = RET_TEST_SWITCH_ERROR;
            TRACE_CODE("��ȡ�豸״̬%dms��ʱ��\n",TIMEOUT_TEST_GET_DEVICESTATUS);
            break;
        }
    }
    return ret;
}

RET_TEST_SWITCH TestProcess_settingButton()
{
    RET_TEST_SWITCH ret = RET_TEST_SWITCH_SUCCESS;

    for(uint8_t i=0; i<3; i++)
    {
        //�������ü�---�½���-------------------------------
        Driver_gpioSet(PORT_TEST_SETTING_BUTTON, PORT_VALUE_HIGH);
        Task_sleep((50*1000)/Clock_tickPeriod);
        Driver_gpioSet(PORT_TEST_SETTING_BUTTON, PORT_VALUE_LOW);
        Task_sleep((50*1000)/Clock_tickPeriod);
        Driver_gpioSet(PORT_TEST_SETTING_BUTTON, PORT_VALUE_HIGH);

        //�ȴ�����setting button�¼�
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_EVENT_SETTINGBUTTON, TIMEOUT_TEST_GET_EVENT);
        if (events & EVENT_TESTPROCESS_EVENT_SETTINGBUTTON)
        {
            TRACE_CODE("���ü��¼���ȡ�ɹ�.\n");
            ret = RET_TEST_SWITCH_SUCCESS;
            break;
        }
        else
        {
            ret = RET_TEST_SWITCH_ERROR;
            TRACE_CODE("���ü��¼���ȡ %dms��ʱ��\n",TIMEOUT_TEST_GET_EVENT);
            break;
        }
    }
    return ret;
}

RET_TEST_SWITCH TestProcess_clearButton()
{
    RET_TEST_SWITCH ret = RET_TEST_SWITCH_SUCCESS;

    for(uint8_t i=0; i<3; i++)
    {
        //�������ü�---�½���-------------------------------
        Task_sleep((50*1000)/Clock_tickPeriod);
        Driver_gpioSet(PORT_TEST_CLEAR_BUTTON, PORT_VALUE_HIGH);
        Task_sleep((50*1000)/Clock_tickPeriod);
        Driver_gpioSet(PORT_TEST_CLEAR_BUTTON, PORT_VALUE_LOW);
        Task_sleep((50*1000)/Clock_tickPeriod);
        Driver_gpioSet(PORT_TEST_CLEAR_BUTTON, PORT_VALUE_HIGH);
        Task_sleep((50*1000)/Clock_tickPeriod);

        //�ȴ�����clear button�¼�
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_EVENT_CLEARBUTTON, TIMEOUT_TEST_GET_EVENT);
        if (events & EVENT_TESTPROCESS_EVENT_CLEARBUTTON)
        {
            TRACE_CODE("��ռ��¼���ȡ�ɹ�.\n");
            ret = RET_TEST_SWITCH_SUCCESS;
            break;
        }
        else
        {
            ret = RET_TEST_SWITCH_ERROR;
            TRACE_CODE("��ռ��¼���ȡ %dms��ʱ��\n",TIMEOUT_TEST_GET_EVENT);
            break;
        }
    }
    return ret;
}

RET_TEST_SWITCH TestProcess_obliqueTongue()
{
    RET_TEST_SWITCH ret = RET_TEST_SWITCH_SUCCESS;

    for(uint8_t i=0; i<3; i++)
    {
        //�������ü�---�½���-------------------------------
        Driver_gpioSet(PORT_TEST_OBLIQUE_LOCK_TONGUE, PORT_VALUE_HIGH);
        Task_sleep((50*1000)/Clock_tickPeriod);
        Driver_gpioSet(PORT_TEST_OBLIQUE_LOCK_TONGUE, PORT_VALUE_LOW);
        Task_sleep((50*1000)/Clock_tickPeriod);
        Driver_gpioSet(PORT_TEST_OBLIQUE_LOCK_TONGUE, PORT_VALUE_HIGH);

        //�ȴ�����б�������¼�
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_EVENT_OBLIQUETONGUE, TIMEOUT_TEST_GET_EVENT);
        if (events & EVENT_TESTPROCESS_EVENT_OBLIQUETONGUE)
        {
            TRACE_CODE("б�������¼���ȡ�ɹ�.\n");
            ret = RET_TEST_SWITCH_SUCCESS;
            break;
        }
        else
        {
            ret = RET_TEST_SWITCH_ERROR;
            TRACE_CODE("б�������¼���ȡ %dms��ʱ��\n",TIMEOUT_TEST_GET_EVENT);
            break;
        }
    }
    return ret;
}