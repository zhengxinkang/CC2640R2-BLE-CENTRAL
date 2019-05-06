/*
 * TestProcess_powerMax.c
 *
 *  Created on: 2018年9月28日
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
        //命令主板进入低功耗
        Task_sleep((200 * 1000) / Clock_tickPeriod);
        uint8_t para = POWER_MODE_SLEEP;
        SendCmd_qcTest_setDevicePower(&para, 1);

        //命令主板进入低功耗
        Task_sleep((100 * 1000) / Clock_tickPeriod);
        SendCmd_qcTest_setDevicePower(&para, 1);

        //将各个IO口设置为不漏电状态
        Task_sleep((100 * 1000) / Clock_tickPeriod);
        SetPin_lowPower();

        //延迟1000ms，预统计一次500ms内的平均电流
        Task_sleep((1000 * 1000) / Clock_tickPeriod);
        uint32_t avgCurrentThis = avgCurrentCount(0, false, 1);
        currentRet = avgCurrentThis;
        if (avgCurrentThis <= minUaTooSmall)
        {
            TRACE_CODE("预检测,休眠功耗为%d uA，低于正常范围，继续统计4秒内平均电流。\n", avgCurrentThis);
            continue;
        }
        else if (avgCurrentThis <= minUa)
        {
            TRACE_CODE("预检测，睡眠功耗确认为“成功”,休眠功耗为%d uA.\n", avgCurrentThis);
            break;
        }
        else if (avgCurrentThis <= preMinUa)
        {
            TRACE_CODE("预检测实时电流为%duA，在【%d-%duA】范围内，继续统计4秒内平均电流。\n",
                       avgCurrentThis, minUa, preMinUa);
        }
        else
        {
            TRACE_ERROR("预检测实时电流为%duA，超过【%duA】，不等待，直接重试。\n", avgCurrentThis,
                        preMinUa);
            TestProcess_resetTarget();
            continue;
        }

        //统计4秒内的平均电流
        Task_sleep((4000 * 1000) / Clock_tickPeriod);
        uint32_t avgCurrent = avgCurrentCount(0, false, 8);
        currentRet = avgCurrent;
        if (avgCurrentThis <= minUaTooSmall)
        {
            TRACE_CODE("睡眠功耗确认为“失败”,休眠功耗为%d uA!!!,再次尝试。\n", avgCurrent);
            TestProcess_resetTarget();
            continue;
        }
        else if (avgCurrent <= minUa)
        {
            TRACE_CODE("睡眠功耗确认为“成功”,休眠功耗为%d uA.\n", avgCurrent);
            break;
        }
        else
        {
            if (time >= MAX_TIME_POWER_CHECK - 1)
            {
                TRACE_CODE("睡眠功耗确认为“失败”,休眠功耗为%d uA!!!尝试%d次后放弃。\n", avgCurrent,
                           MAX_TIME_POWER_CHECK);
                break;
            }
            else
            {
                TRACE_CODE("睡眠功耗确认为“失败”,休眠功耗为%d uA!!!,再次尝试。\n", avgCurrent);
                TestProcess_resetTarget();
                continue;
            }
        }
    }

    //分段结果
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
