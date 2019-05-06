/*
 * TestProcess_powerMax.c
 *
 *  Created on: 2018��9��28��
 *      Author: zheng
 */

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TestProcess_powerMode.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "DataType.h"
#include "TaskTest.h"
#include "Trace.h"
#include "Driver_gpio.h"
#include "Driver_portUart.h"
#include "Driver_uart.h"


#define TIMEOUT_TEST_PROCESS_POWERMODE       6000000//60��
void SetPin_lowPower();

RET_TEST_POWER_MODE TestProcess_powerMode(POWER_MODE powerMode)
{
    RET_TEST_POWER_MODE ret = RET_TEST_POWER_MODE_SUCCESS;
    switch(powerMode)
    {
        case POWER_MODE_SLEEP:
        {
            //���ù���ģʽ
            Task_sleep((200*1000)/Clock_tickPeriod);
            //����֮ǰ���¼�
            TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, 1);
            uint8_t para = POWER_MODE_SLEEP;
            SendCmd_qcTest_setDevicePower(&para, 1);
            Task_sleep((200*1000)/Clock_tickPeriod);
            SetPin_lowPower();

            //�ӳ�500ms
            Task_sleep((500*1000)/Clock_tickPeriod);
            TRACE_DEBUG("<<<<<<�ȴ�˯�߹��Ĳ��Խ��\n");
            TRACE_DEBUG("<<<<<<����ͨ��������С��00.040���밴�¡��ɹ��������Բ�ͨ���밴�¡�ʧ�ܡ�\n");

            //�ȴ�������Ϣ10����
            uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, TIMEOUT_TEST_PROCESS_POWERMODE);
            if (events & EVENT_TESTPROCESS_CONFIRM_SUCCESS)
            {
                ret = RET_TEST_POWER_MODE_SUCCESS;
                TRACE_DEBUG("˯�߹���ȷ��Ϊ���ɹ���.\n");
            }
            else if (events & EVENT_TESTPROCESS_CONFIRM_FAIL)
            {
                ret = RET_TEST_POWER_MODE_ERROR;
                TRACE_DEBUG("˯�߹���ȷ��Ϊ��ʧ�ܡ�.\n");
            }
            else
            {
                ret = RET_TEST_POWER_MODE_ERROR;
                TRACE_DEBUG("˯�߹���ȷ��%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_POWERMODE);
            }
            break;
        }
        case POWER_MODE_WAKE_ALL:
        {
            //���ù���ģʽ
            uint8_t para = POWER_MODE_WAKE_ALL;
            SendCmd_qcTest_setDevicePower(&para, 1);

            //�ӳ�500ms
            Task_sleep((500*1000)/Clock_tickPeriod);
            TRACE_DEBUG("<<<<<<�ȴ�ȫ���ѹ��Ĳ��Խ��������ͨ���밴�³ɹ��������Բ�ͨ���밴��ʧ�ܼ�\n");

            //�ȴ�������Ϣ10����
            uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, TIMEOUT_TEST_PROCESS_POWERMODE);
            if (events & EVENT_TESTPROCESS_CONFIRM_SUCCESS)
            {
                ret = RET_TEST_POWER_MODE_SUCCESS;
                TRACE_DEBUG("ȫ���ѹ���ȷ��Ϊ���ɹ���.\n");
            }
            else if (events & EVENT_TESTPROCESS_CONFIRM_FAIL)
            {
                ret = RET_TEST_POWER_MODE_ERROR;
                TRACE_DEBUG("ȫ���ѹ���ȷ��Ϊ��ʧ�ܡ�.\n");
            }
            else
            {
                ret = RET_TEST_POWER_MODE_ERROR;
                TRACE_DEBUG("ȫ���ѹ���ȷ��%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_POWERMODE);
            }
            break;
        }
        case POWER_MODE_NORMAL:
        {
            //���ù���ģʽ
            uint8_t para = POWER_MODE_NORMAL;
            SendCmd_qcTest_setDevicePower(&para, 1);
            TRACE_DEBUG("��������ģʽ���óɹ�.\n");
            //�ӳ�100ms
            Task_sleep((100*1000)/Clock_tickPeriod);
            break;
        }
    }
    return ret;
}

void SetPin_lowPower()
{
    Driver_gpioSet(PORT_ANTI_LOCK,                  PORT_VALUE_HIGH);
    Driver_gpioSet(PORT_PREVENT_DISMANTLE,          PORT_VALUE_HIGH);
    Driver_gpioSet(PORT_TEST_OBLIQUE_LOCK_TONGUE,   PORT_VALUE_HIGH);
    Driver_gpioSet(PORT_TEST_SETTING_BUTTON,        PORT_VALUE_HIGH);
    Driver_gpioSet(PORT_TEST_CLEAR_BUTTON,          PORT_VALUE_HIGH);
    Driver_gpioSet(PORT_TEST_RESET_TARGET,          PORT_VALUE_HIGH);

    Driver_uart_close();
    Driver_uartGpio_open();
    Driver_uartGpioSetHigh();
}
