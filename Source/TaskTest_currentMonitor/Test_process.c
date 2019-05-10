/*
 * Test_process.c
 *
 *  Created on: 2018��8��30��
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
#include "testCurrentConfig.h"
#include "hal_led.h"
#include "Hal_abnormalRecord.h"


static TEST_PROCESS_STATE test_process_state = TEST_PROCESS_STATE_IDLE;
bool IsBusy_testProcess()
{
    if(TEST_PROCESS_STATE_IDLE != test_process_state)
    {
//        TRACE_ERROR("���󣺲��Գ���æ����ȴ��ϴβ�����ɺ����ԡ�\n");
        return TRUE;
    }
    else
    {
        test_process_state = TEST_PROCESS_STATE_SUCESS;
        return FALSE;
    }
}

#define MAX_TIMES_WAIT  15

//#define PRE_MIN_CURRENT 30
//#define PRE_MAX_CURRENT 90
//
//#define MIN_CURRENT     40
//#define MAX_CURRENT     75

static bool status_test=false;

TEST_PROCESS_STATE Test_process()
{
    Hal_abnormalRecord_init();

    uint32_t events;
    status_test=true;
    for(;;)
    {
        //ȥ����
        TestEvent_pend(0xffffffff, 1);

        events = TestEvent_pend(EVENT_CURRENT_ABNORMAL|EVENT_TESTPROCESS_CONFIRM_SUCCESS, 1000);
        if(events & EVENT_CURRENT_ABNORMAL)
        {
            TRACE_DEBUG("�����쳣�����е���ȷ�ϣ�\n");
            //��4�룬ͳ��4���ڵ�ƽ������
            Task_sleep((8000 * 1000) / Clock_tickPeriod);
            uint32_t avgCurrent = avgCurrentCount(0, false, 8);

            if(avgCurrent>=MIN_CURRENT && avgCurrent<=MAX_CURRENT)
            {
                TRACE_DEBUG("ȷ�Ͻ������������������%duA����\n",avgCurrent);
                Buzz_action(200, 100, 3);
            }
            else
            {
                TRACE_DEBUG("ȷ�Ͻ���������쳣��%duA��,�˳����ԣ�\n",avgCurrent);
                Hal_led_GYR(0, 0, 1);
                Buzz_action(200, 100, 7);

                UI(UI_TYPE_CURRENT_MONITOR, avgCurrent, NULL, 0);
                break;
            }
        }
        else if(events & EVENT_TESTPROCESS_CONFIRM_SUCCESS)
        {
            Buzz_action(500, 100, 1);
            TRACE_DEBUG("�˳����ԣ�\n");
            break;
        }
        else
        {
//            Led_action(200, 100, 1);
        }
    }
    status_test=false;
}

bool Test_process_getStatus()
{
    return status_test;
}
