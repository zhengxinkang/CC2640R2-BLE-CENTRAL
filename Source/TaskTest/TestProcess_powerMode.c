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


#define TIMEOUT_TEST_PROCESS_POWERMODE      60000//60秒
#define MIN_CURRENT                         65
#define MAX_TIME_POWER_CHECK                2
void SetPin_lowPower();

RET_TEST_POWER_MODE TestProcess_powerMode(POWER_MODE powerMode)
{
    RET_TEST_POWER_MODE ret = RET_TEST_POWER_MODE_SUCCESS;
    switch(powerMode)
    {
        case POWER_MODE_SLEEP:
        {
            for(uint8_t time=0; time<MAX_TIME_POWER_CHECK; time++)
            {
                //设置功耗模式
                Task_sleep((200*1000)/Clock_tickPeriod);

                uint8_t para = POWER_MODE_SLEEP;
                SendCmd_qcTest_setDevicePower(&para, 1);
                Task_sleep((200*1000)/Clock_tickPeriod);
                SetPin_lowPower();

                //            TRACE_DEBUG("<<<<<<测试通过（电流小于00.060）请按下【成功】，测试不通过请按下【失败】\n");
                Task_sleep((5000*1000)/Clock_tickPeriod);

                uint32_t avgCurrent = avgCurrentCount(0, false, 8);
                if(avgCurrent <= MIN_CURRENT)
                {
                    ret = RET_TEST_POWER_MODE_SUCCESS;
                    TRACE_CODE("睡眠功耗确认为“成功”,休眠功耗为%d uA.\n", avgCurrent);
                    break;
                }
                else
                {
                    //再次读取
                    TRACE_CODE("睡眠功耗第一次确认失败,休眠功耗为%d uA!再次确认。\n", avgCurrent);
                    Task_sleep((2000*1000)/Clock_tickPeriod);
                    avgCurrent = avgCurrentCount(0, false, 8);
                    if(avgCurrent <= MIN_CURRENT)
                    {
                        ret = RET_TEST_POWER_MODE_SUCCESS;
                        TRACE_CODE("睡眠功耗确认为“成功”,休眠功耗为%d uA.\n", avgCurrent);
                        break;
                    }
                    else
                    {
                        ret = RET_TEST_POWER_MODE_ERROR;
                        if(time >= MAX_TIME_POWER_CHECK-1)
                        {
                            TRACE_CODE("睡眠功耗确认为“失败”,休眠功耗为%d uA!!!尝试%d次后放弃。\n", avgCurrent, MAX_TIME_POWER_CHECK);
                            break;
                        }
                        else
                        {
                            TRACE_CODE("睡眠功耗确认为“失败”,休眠功耗为%d uA!!!,再次尝试。\n", avgCurrent);
                            TestProcess_resetTarget();
                        }
                    }
                }
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
