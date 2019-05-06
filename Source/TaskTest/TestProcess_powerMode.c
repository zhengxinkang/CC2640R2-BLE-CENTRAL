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


#define TIMEOUT_TEST_PROCESS_POWERMODE       6000000//60秒
void SetPin_lowPower();

RET_TEST_POWER_MODE TestProcess_powerMode(POWER_MODE powerMode)
{
    RET_TEST_POWER_MODE ret = RET_TEST_POWER_MODE_SUCCESS;
    switch(powerMode)
    {
        case POWER_MODE_SLEEP:
        {
            //设置功耗模式
            Task_sleep((200*1000)/Clock_tickPeriod);
            //消除之前的事件
            TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, 1);
            uint8_t para = POWER_MODE_SLEEP;
            SendCmd_qcTest_setDevicePower(&para, 1);
            Task_sleep((200*1000)/Clock_tickPeriod);
            SetPin_lowPower();

            //延迟500ms
            Task_sleep((500*1000)/Clock_tickPeriod);
            TRACE_DEBUG("<<<<<<等待睡眠功耗测试结果\n");
            TRACE_DEBUG("<<<<<<测试通过（电流小于00.040）请按下【成功】，测试不通过请按下【失败】\n");

            //等待按键消息10秒钟
            uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, TIMEOUT_TEST_PROCESS_POWERMODE);
            if (events & EVENT_TESTPROCESS_CONFIRM_SUCCESS)
            {
                ret = RET_TEST_POWER_MODE_SUCCESS;
                TRACE_DEBUG("睡眠功耗确认为“成功”.\n");
            }
            else if (events & EVENT_TESTPROCESS_CONFIRM_FAIL)
            {
                ret = RET_TEST_POWER_MODE_ERROR;
                TRACE_DEBUG("睡眠功耗确认为“失败”.\n");
            }
            else
            {
                ret = RET_TEST_POWER_MODE_ERROR;
                TRACE_DEBUG("睡眠功耗确认%dms后超时！\n",TIMEOUT_TEST_PROCESS_POWERMODE);
            }
            break;
        }
        case POWER_MODE_WAKE_ALL:
        {
            //设置功耗模式
            uint8_t para = POWER_MODE_WAKE_ALL;
            SendCmd_qcTest_setDevicePower(&para, 1);

            //延迟500ms
            Task_sleep((500*1000)/Clock_tickPeriod);
            TRACE_DEBUG("<<<<<<等待全唤醒功耗测试结果，测试通过请按下成功键，测试不通过请按下失败键\n");

            //等待按键消息10秒钟
            uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_CONFIRM_SUCCESS|EVENT_TESTPROCESS_CONFIRM_FAIL, TIMEOUT_TEST_PROCESS_POWERMODE);
            if (events & EVENT_TESTPROCESS_CONFIRM_SUCCESS)
            {
                ret = RET_TEST_POWER_MODE_SUCCESS;
                TRACE_DEBUG("全唤醒功耗确认为“成功”.\n");
            }
            else if (events & EVENT_TESTPROCESS_CONFIRM_FAIL)
            {
                ret = RET_TEST_POWER_MODE_ERROR;
                TRACE_DEBUG("全唤醒功耗确认为“失败”.\n");
            }
            else
            {
                ret = RET_TEST_POWER_MODE_ERROR;
                TRACE_DEBUG("全唤醒功耗确认%dms后超时！\n",TIMEOUT_TEST_PROCESS_POWERMODE);
            }
            break;
        }
        case POWER_MODE_NORMAL:
        {
            //设置功耗模式
            uint8_t para = POWER_MODE_NORMAL;
            SendCmd_qcTest_setDevicePower(&para, 1);
            TRACE_DEBUG("正常功耗模式设置成功.\n");
            //延迟100ms
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
