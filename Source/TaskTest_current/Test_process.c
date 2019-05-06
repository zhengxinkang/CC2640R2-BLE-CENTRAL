/*
 * Test_process.c
 *
 *  Created on: 2018年8月30日
 *      Author: zheng
 */
#include <TestProcess_powerMode.h>
#include "TaskTest.h"
#include "Trace.h"
#include "stdbool.h"
#include "string.h"
#include "BoardAction.h"
#include "BF_Util.h"
#include "KeyBoard_action.h"
#include "Hal_oled.h"
#include "UI.h"
#include "Lock_atcion.h"
#include "Test_process.h"
#include "TestProcess_ble.h"
#include "TestProcess_bat.h"
#include "TestProcess_card.h"
#include "TestProcess_keyBoard.h"
#include "TestProcess_motor.h"
#include "TestProcess_resetTarget.h"
#include "TestProcess_setInfo.h"
#include "TestProcess_switch.h"
#include "TestProcess_uartEcho.h"
#include "TestProcess_led.h"
#include "TestProcess_powerMode.h"
#include "TestProcess_self.h"
#include "TestProcess_voice.h"
#include "TestProcess_writeReadTestResult.h"
#include "TestProcess_select.h"
#include "Hal_buzz.h"
#include "Hal_led.h"
#include "Hal_electricCurrent.h"


static TEST_PROCESS_STATE test_process_state = TEST_PROCESS_STATE_IDLE;
bool IsBusy_testProcess()
{
    if(TEST_PROCESS_STATE_IDLE != test_process_state)
    {
//        TRACE_ERROR("错误：测试程序繁忙，请等待上次测试完成后再试。\n");
        return TRUE;
    }
    else
    {
        test_process_state = TEST_PROCESS_STATE_SUCESS;
        return FALSE;
    }
}

#define MAX_TIMES_WAIT  15

#define PRE_MIN_CURRENT 40
#define PRE_MAX_CURRENT 90

#define MIN_CURRENT     40
#define MAX_CURRENT     75


TEST_PROCESS_STATE Test_process()
{
    //     Hal_led_YGR(bool yellow, bool green, bool red);
    Hal_led_GYR(0, 0, 0);
    UI(UI_TYPE_READY, 0, NULL, 0);
    TEST_PROCESS_STATE ret_test = TEST_PROCESS_STATE_SUCESS;
    uint32_t avgCurrentThis, avgCurrentTotal =888,dis_current;
    TRACE_DEBUG("电流测试开始。\n");
    Buzz_action(100, 0, 1);
    for(uint8_t index_pre=0; index_pre<MAX_TIMES_WAIT; index_pre++)
    {
        avgCurrentThis = avgCurrentCount(0, false, 1);

        if(avgCurrentThis >= PRE_MIN_CURRENT&&avgCurrentThis <= PRE_MAX_CURRENT)
        {
            ret_test = TEST_PROCESS_STATE_SUCESS;
            TRACE_DEBUG("本次获取到的实时电流为%duA,在预测范围内，进入平均电流测试。\n", avgCurrentThis);
            break;
        }
        else
        {
            ret_test = TEST_PROCESS_STATE_BAT_FAIL;
            TRACE_DEBUG("本次获取到的实时电流为%duA,bu 不在预测范围内，等待再次采集！\n", avgCurrentThis);
            Buzz_action(50, 0, 1);
            Led_action(100, 0, 1);
        }
        BF_taskSleepMs(1000);
    }

    if(TEST_PROCESS_STATE_SUCESS == ret_test)
    {
        Buzz_action(50, 950, 4);
        Led_action(500, 500, 4);
        BF_taskSleepMs(4000);
        avgCurrentTotal = avgCurrentCount(0, false, 8);
        if(avgCurrentTotal >= MIN_CURRENT&&avgCurrentTotal <= MAX_CURRENT)
        {
            ret_test = TEST_PROCESS_STATE_SUCESS;
            TRACE_DEBUG("平均电流为%duA，通过测试。\n", avgCurrentTotal);
        }
        else
        {
            ret_test = TEST_PROCESS_STATE_USB_FAIL;
            TRACE_DEBUG("平均电流为%duA，测试失败！！！\n", avgCurrentTotal);
        }
    }

    if(TEST_PROCESS_STATE_SUCESS == ret_test)
    {
        Buzz_action(1500, 400, 2);
        Hal_led_GYR(1, 0, 0);

        UI(UI_TYPE_SUCCESS, 0, NULL, avgCurrentTotal);
    }
    else
    {
        Buzz_action(100, 50, 7);
        Hal_led_GYR(0, 0, 1);

        dis_current = avgCurrentTotal;
        if(dis_current>999)
            dis_current = 999;

        UI(UI_TYPE_FAIL, 0, NULL, avgCurrentTotal);
    }

    return ret_test;
}
