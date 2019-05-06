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
#include "Hal_electricCurrent.h"
#include "TestProcess_resetTarget.h"

#define MAX_TIME_POWER_CHECK                3
void SetPin_lowPower();

uint8_t TestProcess_powerModeMin(uint8_t minUaTooSmall, uint8_t minUa, uint8_t preMinUa)
{
    uint8_t ret = RET_TEST_POWER_MODE_MIN_SUCCESS;
    uint8_t currentRet = 0;
    for (uint8_t time = 0; time < MAX_TIME_POWER_CHECK; time++)
    {
        //�����������͹���
        Task_sleep((200 * 1000) / Clock_tickPeriod);
        uint8_t para = POWER_MODE_SLEEP;
        SendCmd_qcTest_setDevicePower(&para, 1);

        //�����������͹���
        Task_sleep((100 * 1000) / Clock_tickPeriod);
        SendCmd_qcTest_setDevicePower(&para, 1);

        //������IO������Ϊ��©��״̬
        Task_sleep((100 * 1000) / Clock_tickPeriod);
        SetPin_lowPower();

        //�ӳ�1000ms��Ԥͳ��һ��500ms�ڵ�ƽ������
        Task_sleep((1000 * 1000) / Clock_tickPeriod);
        uint32_t avgCurrentThis = avgCurrentCount(0, false, 1);
        currentRet = avgCurrentThis;
        if (avgCurrentThis <= minUaTooSmall)
        {
            TRACE_CODE("Ԥ���,���߹���Ϊ%d uA������������Χ������ͳ��4����ƽ��������\n", avgCurrentThis);
            continue;
        }
        else if (avgCurrentThis <= minUa)
        {
            TRACE_CODE("Ԥ��⣬˯�߹���ȷ��Ϊ���ɹ���,���߹���Ϊ%d uA.\n", avgCurrentThis);
            break;
        }
        else if (avgCurrentThis <= preMinUa)
        {
            TRACE_CODE("Ԥ���ʵʱ����Ϊ%duA���ڡ�%d-%duA����Χ�ڣ�����ͳ��4����ƽ��������\n",
                       avgCurrentThis, minUa, preMinUa);
        }
        else
        {
            TRACE_ERROR("Ԥ���ʵʱ����Ϊ%duA��������%duA�������ȴ���ֱ�����ԡ�\n", avgCurrentThis,
                        preMinUa);
            TestProcess_resetTarget();
            continue;
        }

        //ͳ��4���ڵ�ƽ������
        Task_sleep((4000 * 1000) / Clock_tickPeriod);
        uint32_t avgCurrent = avgCurrentCount(0, false, 8);
        currentRet = avgCurrent;
        if (avgCurrentThis <= minUaTooSmall)
        {
            TRACE_CODE("˯�߹���ȷ��Ϊ��ʧ�ܡ�,���߹���Ϊ%d uA!!!,�ٴγ��ԡ�\n", avgCurrent);
            TestProcess_resetTarget();
            continue;
        }
        else if (avgCurrent <= minUa)
        {
            TRACE_CODE("˯�߹���ȷ��Ϊ���ɹ���,���߹���Ϊ%d uA.\n", avgCurrent);
            break;
        }
        else
        {
            if (time >= MAX_TIME_POWER_CHECK - 1)
            {
                TRACE_CODE("˯�߹���ȷ��Ϊ��ʧ�ܡ�,���߹���Ϊ%d uA!!!����%d�κ������\n", avgCurrent,
                           MAX_TIME_POWER_CHECK);
                break;
            }
            else
            {
                TRACE_CODE("˯�߹���ȷ��Ϊ��ʧ�ܡ�,���߹���Ϊ%d uA!!!,�ٴγ��ԡ�\n", avgCurrent);
                TestProcess_resetTarget();
                continue;
            }
        }
    }

    //�ֶν��
    if (currentRet <= 98)
    {
        ret = currentRet;
    }
    else
    {
        ret = RET_TEST_POWER_MODE_MIN_POWER_OVERFLOW;
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
