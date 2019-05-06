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

static uint8_t macAddr[6];

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

uint8_t checkResult[50];
uint8_t checkSelfResult[LEN_SELF_TEST_RESULT];
#define TOTAL_TEST_ITEM     22
                                            //0     //1     //2     //3     //4     //5     //6     //7     //8     //9     //10    //11    //12    //13    //14    //15    //16    //17    //18    //19    //20     //21
                                            //USB电  //电池    //回声    //擦除    //配写    //配验    //自测    //正转    //反转    //反锁    //防撬    //设置键//清空键  //斜舌    //刷卡   //键盘    //语音    //LED   //BLE   //低功耗//写结果   //读结果
//全面测试
uint8_t processActive[TOTAL_TEST_ITEM] = {  1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1};
//低功耗测试
//uint8_t processActive[TOTAL_TEST_ITEM] = {  1,      0,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      1,      0,      0,      0,      0,      0};
//BLE
//uint8_t processActive[TOTAL_TEST_ITEM] = {  1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      1,      0,      0};
//LED
//uint8_t processActive[TOTAL_TEST_ITEM] = {  1,      1,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      1,      0,      0,      0};

                                                   //0     //1     //2     //3     //4     //5     //6
//自测项目                                                                          //BLE   //ZIG   //EEPROM//指纹    //FLASH //RTC   //ADC
uint8_t processSelfActive[LEN_SELF_TEST_RESULT] = { 1,      1,      1,      1,      1,      1,      1};

#define     GOTO_TESTEND  {goto testEnd;}
#define     GOTO_TESTOUT  {goto testOut;}
TEST_PROCESS_STATE Test_process()
{
    //测试开始
    memset(checkResult, 0x00, sizeof(checkResult));
    memset(checkSelfResult, 0x01, sizeof(checkSelfResult));

    uint8_t testItems = 0;

    //获取检测机 设置
    TARGET_MODEL targetModel = TargetModel_get();
    if(TARGET_MODEL_ERROR == targetModel)
    {
        test_process_state = TEST_PROCESS_STATE_IDLE;
        return test_process_state;
        GOTO_TESTEND
    }

    TRACE_DEBUG("---测试开始---------\n");
    UI(UI_TYPE_READY, 0, NULL, 0);
    //0测试项---USB电源检测--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-usb power   ", 0);
        test_process_state = TEST_PROCESS_STATE_USB;
        TRACE_DEBUG("---%d---USB电源检测-----------",testItems);
        RET_TEST_BAT ret_bat = TestProcess_usb();
        checkResult[testItems] = ret_bat;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>USB电源检测：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>USB电源检测：失败！错误码：%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_USB_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //1测试项---电池电源检测------------------------------------------------------------
    test_process_state = TEST_PROCESS_STATE_BAT;
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-bat power  ", 0);
        TRACE_DEBUG("---%d---电池电源检测----------",testItems);
        RET_TEST_BAT ret_bat = TestProcess_bat();
        checkResult[testItems] = ret_bat;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>电池电源检测：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>电池电源检测：失败！错误码：%d!\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_BAT_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //复位目标
    TestProcess_resetTarget();

    //2测试项---UART测试通讯--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-uart        ", 0);
        test_process_state = TEST_PROCESS_STATE_UART_ECHO;
        TRACE_DEBUG("---%d---UART测试通讯----------",testItems);
        RET_TEST_UARTECHO ret_echo = TestProcess_uartEcho();
        checkResult[testItems] = ret_echo;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>UART测试通讯：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>UART测试通讯：失败！错误码：%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_UART_ECHO_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //---读取上次的测试结果--------------------------------------------------------------------
    {
        TRACE_DEBUG("---0000---读取上次的测试结果---------");
        char testResult;
        RET_TEST_WRITEREAD_TESTRESULT ret_readTestResult = TestProcess_readTestResult(&testResult);
        if(0x00 == ret_readTestResult)
        {
            if(TEST_RESULT_SUCCESS == testResult)
            {
                UI(UI_TYPE_TESTED_PASS, 0, NULL, 0);
                TRACE_DEBUG(">>>读取上次的测试结果：已经测试过了，上次测试结果为：成功。\n");
                TRACE_DEBUG(">>>需要重新测试请按：重测。\n>>>不需重新测试请按：退出。\n");
                if(RET_TEST_SELECT_SUCCESS != TestProcess_select())
                {
                    GOTO_TESTOUT
                }
            }
            else if(TEST_RESULT_EMPTY == testResult)
            {
                TRACE_DEBUG(">>>读取上次的测试结果：未测试过，进行第一次测试。\n");
            }
            else
            {
                UI(UI_TYPE_TESTED_FAIL, testResult, NULL, 0);
                TRACE_DEBUG(">>>读取上次的测试结果：测试过了，上次测试失败（%d）！\n",testResult);
                TRACE_DEBUG(">>>需要重新测试请按：重测。\n>>>不需重新测试请按：退出。\n");
                if(RET_TEST_SELECT_SUCCESS != TestProcess_select())
                {
                    GOTO_TESTOUT
                }
            }
        }
        else
        {
            TRACE_DEBUG(">>>读取上次测试结果：失败！错误码：%d.\n\n",testResult);
            GOTO_TESTEND
        }
    }

    //3测试项---擦除EEPROM--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-erase eeprom", 0);
        test_process_state = TEST_PROCESS_STATE_ERASE_EEPROM;
        TRACE_DEBUG("---%d---擦除EEPROM------------",testItems);
        RET_TEST_WRITEREAD_TESTRESULT ret_eraseTestResult = TestProcess_clearTestResult(test_process_state);
        checkResult[testItems] = ret_eraseTestResult;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>擦除EEPROM结果：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>擦除EEPROM结果：失败！错误码：%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_ERASE_EEPROM_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //4测试项---配置写入--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-settingWrite", 0);
        test_process_state = TEST_PROCESS_STATE_SET_INFO;
        TRACE_DEBUG("---%d---配置写入--------------",testItems);
        RET_TEST_SETINFO ret_setInfo = TestProcess_setInfo(targetModel);
        checkResult[testItems] = ret_setInfo;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>配置写入：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>配置写入：失败！错误码：%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_SET_INFO_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //5测试项---配置校验--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-settingCheck", 0);
        test_process_state = TEST_PROCESS_STATE_SET_INFO;
        TRACE_DEBUG("---%d---配置校验--------------",testItems);
        RET_TEST_SETINFO ret_setInfo = TestProcess_readInfo(targetModel);
        checkResult[testItems] = ret_setInfo;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>配置校验：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>配置校验：失败！错误码：%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_CHECK_INFO_FAIL;
            GOTO_TESTEND
        }
    }
    //写完配置，需要复位，否则目标板不会生效，复位目标
    TestProcess_resetTarget();
    testItems++;

    //6测试项---锁自测------------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-lockSelfChk ", 0);
        //自测：目标板上电需要大约4秒时间进行自测
        test_process_state = TEST_PROCESS_STATE_SELFCHECK;
        TRACE_DEBUG("---%d---锁自测----------------",testItems);
//        RET_TEST_SELF ret_self = TestProcess_self(targetModel);
        RET_TEST_SELF ret_self = TestProcess_self();
        if(RET_TEST_SELF_SUCCESS == ret_self)
        {
            memcpy(checkSelfResult, selfTestResult, LEN_SELF_TEST_RESULT);
            if(TARGET_MODEL_D3111 == targetModel)
            {
                //D31-11不带zigbee模块，跳过zigbee模块检测
                checkSelfResult[1] = 0x01;
            }
            else if(TARGET_MODEL_D3100 == targetModel)
            {
                //D31-00不带zigbee模块，跳过zigbee模块检测
                checkSelfResult[1] = 0x01;
                //D31-00不带指纹模块，跳过指纹模块检测
                checkSelfResult[3] = 0x01;
            }
            for(uint8_t i=0; i<LEN_SELF_TEST_RESULT; i++)
            {
                if(0x01 != checkSelfResult[i])
                {
                    switch(i)
                    {
                        case 0:
                        {
                            TRACE_DEBUG("\n自测失败项目：MCU与BLE的串口通信。\n");
                            break;
                        }
                        case 1:
                        {
                            TRACE_DEBUG("\n自测失败项目：MCU与zigbee的串口通信。\n");
                            break;
                        }
                        case 2:
                        {
                            TRACE_DEBUG("\n自测失败项目：MCU与EEPROM的串口通信。\n");
                            break;
                        }
                        case 3:
                        {
                            TRACE_DEBUG("\n自测失败项目：MCU与指纹模块的串口通信。\n");
                            break;
                        }
                        case 4:
                        {
                            TRACE_DEBUG("\n自测失败项目：MCU与外挂FLASH的串口通信。\n");
                            break;
                        }
                        case 5:
                        {
                            TRACE_DEBUG("\n自测失败项目：MCU与RTC的串口通信。\n");
                            break;
                        }
                        case 6:
                        {
                            TRACE_DEBUG("\n自测失败项目：MCU的ADC的测试。\n");
                            break;
                        }
                    }
                    test_process_state = TEST_PROCESS_STATE_SELFCHECK_FAIL;
                }
            }
        }
        else
        {

            test_process_state = TEST_PROCESS_STATE_SELFCHECK_FAIL;
            TRACE_DEBUG(">>>锁自测：失败！获取锁自测结果失败！\n\n",);
            memset(checkSelfResult, 0xff, LEN_SELF_TEST_RESULT);
        }

        if(TEST_PROCESS_STATE_SELFCHECK_FAIL == test_process_state)
        {
            test_process_state = TEST_PROCESS_STATE_SELFCHECK_FAIL;
            GOTO_TESTEND
        }
        else
        {
            TRACE_DEBUG(">>>锁自测：成功.\n\n");
        }
    }
    testItems ++;

    //7测试项---电机正转测试--------------------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-motor Zhen  ", 0);
        test_process_state = TEST_PROCESS_STATE_MOTOR_FORWARD;
        TRACE_DEBUG("---%d---电机正转测试----------",testItems);
        RET_TEST_MOTOR ret_motor = TestProcess_motorForward();
        //失败，重测一次
        if(0x00 != ret_motor)
            ret_motor = TestProcess_motorForward();
        checkResult[testItems] = ret_motor;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>电机正转测试：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>电机正转测试：失败！错误码：%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_MOTOR_FORWARD_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //8测试项---电机反转测试---------------------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-motor Fan   ", 0);
        test_process_state = TEST_PROCESS_STATE_MOTOR_REVERSAL;
        TRACE_DEBUG("---%d---电机反转测试----------",testItems);
        RET_TEST_MOTOR ret_motor = TestProcess_motorReversal();
        //失败，重测一次
        if(0x00 != ret_motor)
            ret_motor = TestProcess_motorReversal();
        checkResult[testItems] = ret_motor;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>电机反转测试：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>电机反转测试：失败！错误码：%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_MOTOR_REVERSAL_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //9测试项---反锁旋钮检测--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-anti lock   ", 0);
        test_process_state = TEST_PROCESS_STATE_ANTI_LOCK;
        TRACE_DEBUG("---%d---反锁旋钮检测----------",testItems);
        RET_TEST_SWITCH ret_antiLock = TestProcess_antiLock();
        checkResult[testItems] = ret_antiLock;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>反锁旋钮检测：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>反锁旋钮检测：失败！错误码：%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_ANTI_LOCK_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //10测试项---防撬开关检测--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-fanqiao     ", 0);
        test_process_state = TEST_PROCESS_STATE_PREVENT_DISMANTLE;
        TRACE_DEBUG("---%d---防撬开关检测---------",testItems);
        RET_TEST_SWITCH ret_preventDismantle = TestProcess_preventDismantle();
        checkResult[testItems] = ret_preventDismantle;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>防撬开关检测：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>防撬开关检测：失败！错误码：%d.\n\n",
                        checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_PREVENT_DISMANTLE_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //11测试项---设置键--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-set btn     ", 0);
        test_process_state = TEST_PROCESS_STATE_SETTING_BUTTON;
        TRACE_DEBUG("---%d---设置键---------------",testItems);
        RET_TEST_SWITCH ret_settingButton = TestProcess_settingButton();
        checkResult[testItems] = ret_settingButton;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>设置键：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>设置键：失败！错误码：%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_SETTING_BUTTON_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //12测试项---清空键--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-clear btn   ", 0);
        test_process_state = TEST_PROCESS_STATE_CLEAR_BUTTON;
        TRACE_DEBUG("---%d---清空键---------------",testItems);
        RET_TEST_SWITCH ret_clearButton = TestProcess_clearButton();
        checkResult[testItems] = ret_clearButton;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>清空键：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>清空键：失败！错误码：%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_CLEAR_BUTTON_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //13测试项---斜舌--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-xieshe      ", 0);
        test_process_state = TEST_PROCESS_STATE_OBLIQUE_TONGUE;
        TRACE_DEBUG("---%d---斜舌-----------------",testItems);
        RET_TEST_SWITCH ret_obliqueTonque = TestProcess_obliqueTongue();
        checkResult[testItems] = ret_obliqueTonque;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>斜舌：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>斜舌：失败！错误码：%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_OBLIQUE_TONGUE_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //14测试项---刷卡测试----------------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-card        ", 0);
        test_process_state = TEST_PROCESS_STATE_CARD;
        TRACE_DEBUG("---%d---刷卡测试-------------",testItems);
        RET_TEST_CARD ret_card = TestProcess_card();
        checkResult[testItems] = ret_card;
        if (0x00 == checkResult[testItems])
         {
             TRACE_DEBUG(">>>刷卡测试：成功.\n\n");
         }
         else
         {
             TRACE_DEBUG(">>>刷卡测试：失败！错误码：%d.\n\n", checkResult[testItems]);
             test_process_state = TEST_PROCESS_STATE_CARD_FAIL;
             GOTO_TESTEND
         }
    }
    testItems++;

    //15测试项---键盘测试-------------------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-touch Btn   ", 0);
        test_process_state = TEST_PROCESS_STATE_KEYBOARD;
        TRACE_DEBUG("---%d---键盘测试-------------",testItems);
        RET_TEST_KEYBOARD ret_keyBoard = TestProcess_keyBoard();
        checkResult[testItems] = ret_keyBoard;
        if (0x00 == checkResult[testItems])
         {
             TRACE_DEBUG(">>>键盘测试：成功.\n\n");
         }
         else
         {
             TRACE_DEBUG(">>>键盘测试：失败！错误码：%d.\n\n", checkResult[testItems]);
             test_process_state = TEST_PROCESS_STATE_KEYBOARD_FAIL;
             GOTO_TESTEND
         }
    }
    testItems++;

    //16测试项---语音测试--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-voice       ", 0);
        test_process_state = TEST_PROCESS_STATE_VOICE;
        TRACE_DEBUG("---%d---语音测试-------------",testItems);
        RET_TEST_VOICE ret_voice = TestProcess_voice();
        checkResult[testItems] = ret_voice;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>语音测试：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>语音测试：失败！错误码：%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_VOICE_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //17测试项---LED灯测试--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-led         ", 0);
        test_process_state = TEST_PROCESS_STATE_LED;
        TRACE_DEBUG("---%d---LED灯测试------------",testItems);
        RET_TEST_LED ret_led = TestProcess_led();
        checkResult[testItems] = ret_led;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>LED灯测试：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>LED灯测试：失败！错误码：%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_LED_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //18测试项---BLE模块与手机通信测试--------------------------------------------------------------------
    if(processActive[testItems])

    {
        UI(UI_TYPE_PROCESS, testItems, "-ble         ", 0);
        test_process_state = TEST_PROCESS_STATE_BLE;
        TRACE_DEBUG("---%d---BLE通信测试----------",testItems);
        RET_TEST_BLE ret_ble = TestProcess_bleByMacAddr(NULL, 40, 6, 3, 3);
        checkResult[testItems] = ret_ble;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>BLE通信测试：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>BLE通信测试：失败！错误码：%d.\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_BLE_FAIL;
            switch(ret_ble)
            {
                case RET_TEST_BLE_TIMEOUT:
                {
                    TRACE_DEBUG(">>>失败原因：未知.\n\n",);
                    break;
                }
                case RET_TEST_BLE_DISCOVERTIMEOUT:
                {
                    TRACE_DEBUG(">>>失败原因：搜索超时.\n\n",);
                    break;
                }
                case RET_TEST_BLE_NOFIND:
                {
                    TRACE_DEBUG(">>>失败原因：未找到BLE.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_MTUSIZE:
                {
                    TRACE_DEBUG(">>>失败原因：MTU size.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_PROCEDURE_COMPLETE:
                {
                    TRACE_DEBUG(">>>失败原因：未知.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_CHARACTERISTIC:
                {
                    TRACE_DEBUG(">>>失败原因：关键字获取失败.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_CONNECT:
                {
                    TRACE_DEBUG(">>>失败原因：连接失败.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_DATATRANS:
                {
                    TRACE_DEBUG(">>>失败原因：数据传输失败.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_GETBLEINFO:
                {
                    TRACE_DEBUG(">>>失败原因：获取BLE信息失败.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_ACTIVEBLE:
                {
                    TRACE_DEBUG(">>>失败原因：蓝牙模块唤醒失败.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_NOTINIT:
                {
                    TRACE_DEBUG(">>>失败原因：蓝牙模块未初始化，请使用《智辉空间App》进行出厂.\n\n",);
                    break;
                }
            }
            GOTO_TESTEND
        }
    }
    testItems++;

#define CURRENT_SLEEP_DOWN      30  //休眠电流下限
#define CURRENT_SLEEP_UP        55  //休眠电流上限
#define CURRENT_SLEEP_PRE       98 //预估电流上限，有必要延长时间再测一次
    uint8_t currentSuccess = 0;
    //19测试项---休眠功耗测试--------------------------------------------------------------------
    if(processActive[testItems])
    {
        UI(UI_TYPE_PROCESS, testItems, "-min power   ", 0);
        test_process_state = TEST_PROCESS_STATE_POWER_MIN;
        TRACE_DEBUG("---%d---休眠功耗测试---------",testItems);
        uint8_t ret_powerModeSleep = TestProcess_powerModeMin(CURRENT_SLEEP_DOWN, CURRENT_SLEEP_UP, CURRENT_SLEEP_PRE);
        //转换测试结果
        if(ret_powerModeSleep < CURRENT_SLEEP_DOWN)
        {
            //电流小于30uA
            checkResult[testItems] = ret_powerModeSleep;
        }
        else if(ret_powerModeSleep < CURRENT_SLEEP_UP)
        {
            //电流30-55uA
            checkResult[testItems] = 0x00;
        }
        else if(ret_powerModeSleep < CURRENT_SLEEP_PRE)
        {
            //电流56-98uA
            checkResult[testItems] = ret_powerModeSleep;
        }
        else
        {
            //电流大于98uA
            checkResult[testItems] = RET_TEST_POWER_MODE_MIN_POWER_OVERFLOW;
        }

        //处理测试结果
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>休眠功耗测试：成功.\n\n");
            currentSuccess = ret_powerModeSleep;
        }
        else
        {
            TRACE_DEBUG(">>>休眠功耗测试：失败！错误码：%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_POWER_MAX_FAIL;
            GOTO_TESTEND
        }
        //复位目标-休眠后无法唤醒，只能重启后继续流程接下去
//        if(processActive[testItems+1])
            TestProcess_resetTarget();
    }
    testItems++;

    //统计测试错误结果--------------------------
    testEnd:
    uint8_t testRecord = TEST_RESULT_SUCCESS;
    for(uint8_t i = 0; i<TOTAL_TEST_ITEM; i++)
    {
        if( processActive[i] && (0x00 != checkResult[i]) )
        {
            TRACE_DEBUG("测试结果失败项目：%d。\n",i);
            TRACE_DEBUG("失败代码为：%d。\n",checkResult[i]);
            testRecord = i;
            break;
        }
    }

    if(testRecord == TEST_RESULT_SUCCESS)
    {
        if(0xff == checkSelfResult[0])
        {
            TRACE_DEBUG("自测结果获取失败：%d。\n");
            testRecord = 69;
        }
        else
        {
            for(uint8_t i = 0; i<LEN_SELF_TEST_RESULT; i++)
            {
                if( processSelfActive[i] && (0x01 != checkSelfResult[i]) )
                {
                    TRACE_DEBUG("自测结果失败项目：%d。\n",i);
                    TRACE_DEBUG("失败代码为：%d。\n",checkSelfResult[i]);
                    testRecord = 60 + i;
                    break;
                }
            }
        }
    }

    //20测试项---写入测试结果--------------------------------------------------------------------
    testItems = 20;
    if(processActive[testItems])
    {
//        UI(UI_TYPE_PROCESS, testItems, "-ret write    ");
    //    test_process_state = TEST_PROCESS_STATE_WRITE_TESTRESULT;
        TRACE_DEBUG("---%d---写入测试结果---------",testItems);
        RET_TEST_WRITEREAD_TESTRESULT ret_writeTestResult = TestProcess_writeTestResult(testRecord);
        checkResult[testItems] = ret_writeTestResult;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>写入测试结果：成功.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>写入测试结果：失败！错误码：%d.\n\n",checkResult[testItems]);
    //        test_process_state = TEST_PROCESS_STATE_WRITE_TESTRESULT_FAIL;
        }
    }
    testItems++;

    //21测试项---读取测试结果--------------------------------------------------------------------
    if(processActive[testItems] && 0x00==checkResult[testItems-1])
    {
//        UI(UI_TYPE_PROCESS, testItems, "-ret check    ");
    //    test_process_state = TEST_PROCESS_STATE_READ_TESTRESULT;
        TRACE_DEBUG("---%d---读取测试结果---------",testItems);
        char testResult;
        RET_TEST_WRITEREAD_TESTRESULT ret_readTestResult = TestProcess_readTestResult(&testResult);
        checkResult[testItems] = ret_readTestResult;
        if(0x00 == checkResult[testItems])
        {
            if(testResult != testRecord)
            {
                TRACE_DEBUG(">>>读取测试结果：对比失败！%d\n\n",testResult);
                test_process_state = TEST_PROCESS_STATE_READ_TESTRESULT_FAIL;
            }
            else
            {
                TRACE_DEBUG(">>>读取测试结果：对比成功.\n\n");
            }
        }
        else
        {
            TRACE_DEBUG(">>>读取测试结果：失败！错误码：%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_READ_TESTRESULT_FAIL;
        }
    }
    testItems++;

    //判断测试结果---------------------------------------------------------
    if(testRecord == TEST_RESULT_SUCCESS)
    {
        UI(UI_TYPE_SUCCESS, 0, NULL, currentSuccess);
        TRACE_DEBUG("总测试结果为==================================================|>>>成功<<<|。\n");
    }
    else
    {
        TRACE_DEBUG("总测试结果为**************************************************|失败！失败项目（%d）！\n",testRecord);
        UI(UI_TYPE_FAIL, testRecord, NULL, checkResult[testRecord]);
    }


    //测试结束---------------------------------------------------------------------------------
    testOut:

    test_process_state = TEST_PROCESS_STATE_IDLE;
    TRACE_DEBUG("---测试结束---------\n");
    TRACE_CODE("---删除所有事件---------\n");
    TestEvent_pend(0xffffffff, 1);
    return test_process_state;
}

void AddBle_process()
{
    Lock_action_power(true,false);
    BoardAction_setModeNormal();
    BF_taskSleepMs(4000);
    KeyBoard_wake();
    BF_taskSleepMs(500);
    KeyBoard_wake();
    BF_taskSleepMs(500);
    KeyBoard_wake();
}

void Test_process_setMacAddr(uint8_t* addr)
{
    memcpy(macAddr, addr, 6);
}
