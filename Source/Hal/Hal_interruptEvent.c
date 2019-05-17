/*
 * Hal_interruptEvent.c
 *
 *  Created on: 2018年11月24日
 *      Author: zheng
 */

#include "Lock_atcion.h"
#include "KeyBoard_action.h"
#include "MyEventConfig.h"
#include "Test_process.h"
#include "TaskTest.h"
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "Hal_oled.h"
/*
 * 注意：本文只允许短时间发送事件等操作，不允许长时间操作。
*/

//static bool startStatus = false;
//----------------------------------------------------
void SwInt1_pressDown()
{
//    TestEvent_post(EVENT_ADD_CARD);
//    Hal_oled_stateChange("Add card      ");
}

void SwInt2_preesDown()
{
    TestEvent_post(EVENT_TESTPROCESS_CONFIRM_SUCCESS);
//    Lock_action_bolt(500);
//    TestEvent_post(EVENT_TRY_PW);
//    Hal_oled_stateChange("Try pw        ");
}

void SwInt3_preesDown()
{
    TestEvent_post(EVENT_START_TEST);
//    Lock_action_cardActive(1000);
//    if (startStatus==false && TEST_PROCESS_STATE_IDLE == Test_processStatusGet())
//    {
//        Hal_oled_stateChange("Test start    ");
//        TestEvent_post(EVENT_START_TEST);
//        startStatus = true;
//    }
//    else
//    {
//        if(TEST_PROCESS_STATE_IDLE != Test_processStatusGet())
//            Hal_oled_stateChange("Test running  ");
//        else
//            Hal_oled_stateChange("Please clear  ");
//    }
}

//----------------------------------------------------
void SwInt1_preesUp()
{

}

void SwInt2_preesUp()
{

}

void SwInt3_preesUp()
{

}

//----------------------------------------------------
void SwInt1_press2S()
{
    TestEvent_post(EVENT_TESTPROCESS_CONFIRM_FAIL);
//    static bool isLocked;
//    isLocked = !isLocked;
//    Lock_action_relock(isLocked);
}

void SwInt2_press2S()
{
//    static bool isActive;
//    isActive = !isActive;
//    Lock_action_fq(isActive);
}

void SwInt3_press2S()
{
//    static bool isPowerOn;
//    isPowerOn = !isPowerOn;
//    Lock_action_power(isPowerOn, isPowerOn);
//    void Lock_action_cardActive(uint16_t ms);
//    if (TEST_PROCESS_STATE_IDLE == Test_processStatusGet())
//    {
//        Hal_oled_stateChange("Test clear    ");
//        startStatus = false;
//        Hal_oled_dataChange(OLED_DATA_TYPE_CARD_TOTAL, 0);
//        Hal_oled_dataChange(OLED_DATA_TYPE_CARD_SUCCESS,0);
//        Hal_oled_dataChange(OLED_DATA_TYPE_PASSWORD_TOTAL, 0);
//        Hal_oled_dataChange(OLED_DATA_TYPE_PASSWORD_SUCCESS,0);
//        Hal_oled_dataChange(OLED_DATA_TYPE_BLE_TOTAL, 0);
//        Hal_oled_dataChange(OLED_DATA_TYPE_BLE_SUCCESS,0);
//    }
//    else
//    {
//        Hal_oled_stateChange("Test running  ");
//    }
}
