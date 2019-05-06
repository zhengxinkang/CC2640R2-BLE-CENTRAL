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
//        TRACE_ERROR("���󣺲��Գ���æ����ȴ��ϴβ�����ɺ����ԡ�\n");
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
                                            //USB��  //���    //����    //����    //��д    //����    //�Բ�    //��ת    //��ת    //����    //����    //���ü�//��ռ�  //б��    //�͹���//ˢ��   //����    //����    //LED   //BLE   //д���   //�����
//ȫ�����
uint8_t processActive[TOTAL_TEST_ITEM] = {  1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1};
//�͹��Ĳ���
//uint8_t processActive[TOTAL_TEST_ITEM] = {  1,      0,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      1,      0,      0,      0,      0,      0,      0,      0};
//BLE
//uint8_t processActive[TOTAL_TEST_ITEM] = {  1,      0,      1,      0,      0,      0,      1,      0,      0,      0,      0,      0,      0,      0,      1,      0,      0,      0,      0,      1,      0,      0};

                                                   //0     //1     //2     //3     //4     //5     //6
//�Բ���Ŀ                                                                          //BLE   //ZIG   //EEPROM//ָ��    //FLASH //RTC   //ADC
uint8_t processSelfActive[LEN_SELF_TEST_RESULT] = { 1,      1,      1,      1,      1,      1,      1};

#define     GOTO_TESTEND //{goto testEnd;}
TEST_PROCESS_STATE Test_process()
{
    //���Կ�ʼ
    memset(checkResult, 0x00, sizeof(checkResult));
    uint8_t testItems = 0;

    //��ȡ���� ����
    TARGET_MODEL targetModel = TargetModel_get();
    if(TARGET_MODEL_ERROR == targetModel)
    {
        test_process_state = TEST_PROCESS_STATE_IDLE;
        return test_process_state;
        GOTO_TESTEND
    }

    TRACE_DEBUG("---���Կ�ʼ---------\n");
    //0������---USB��Դ���--------------------------------------------------------------------
    //TODO:�л���USB��Դ
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_USB;
        TRACE_DEBUG("---%d---USB��Դ���-----------",testItems);
        RET_TEST_BAT ret_bat = TestProcess_usb();
        checkResult[testItems] = ret_bat;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>USB��Դ��⣺�ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>USB��Դ��⣺ʧ�ܣ������룺%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_USB_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //1������---��ص�Դ���------------------------------------------------------------
    //TODO:�л�����ص�Դ
    test_process_state = TEST_PROCESS_STATE_BAT;
    if(processActive[testItems])
    {
        TRACE_DEBUG("---%d---��ص�Դ���----------",testItems);
        RET_TEST_BAT ret_bat = TestProcess_bat();
        checkResult[testItems] = ret_bat;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>��ص�Դ��⣺�ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>��ص�Դ��⣺ʧ�ܣ������룺%d!\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_BAT_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //��λĿ��
    TestProcess_resetTarget();

    //2������---UART����ͨѶ--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_UART_ECHO;
        TRACE_DEBUG("---%d---UART����ͨѶ----------",testItems);
        RET_TEST_UARTECHO ret_echo = TestProcess_uartEcho();
        checkResult[testItems] = ret_echo;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>UART����ͨѶ���ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>UART����ͨѶ��ʧ�ܣ������룺%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_UART_ECHO_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //---��ȡ�ϴεĲ��Խ��--------------------------------------------------------------------
    {
        TRACE_DEBUG("---0000---��ȡ�ϴεĲ��Խ��---------");
        char testResult;
        RET_TEST_WRITEREAD_TESTRESULT ret_readTestResult = TestProcess_readTestResult(&testResult);
        if(0x00 == ret_readTestResult)
        {
            if(TEST_RESULT_SUCCESS == testResult)
            {
                TRACE_DEBUG(">>>��ȡ�ϴεĲ��Խ�����Ѿ����Թ��ˣ��ϴβ��Խ��Ϊ���ɹ���\n");
                TRACE_DEBUG(">>>��Ҫ���²����밴���ɹ���\n>>>�������²����밴��ʧ�ܡ�\n");
                if(RET_TEST_SELECT_SUCCESS != TestProcess_select())
                {
                    goto testOut;
                }
            }
            else if(TEST_RESULT_EMPTY == testResult)
            {
                TRACE_DEBUG(">>>��ȡ�ϴεĲ��Խ����δ���Թ������е�һ�β��ԡ�\n");
            }
            else
            {
                TRACE_DEBUG(">>>��ȡ�ϴεĲ��Խ�������Թ��ˣ��ϴβ���ʧ�ܣ�%d����\n",testResult);
                TRACE_DEBUG(">>>��Ҫ���²����밴���ɹ���\n>>>�������²����밴��ʧ�ܡ�\n");
                if(RET_TEST_SELECT_SUCCESS != TestProcess_select())
                {
                    goto testOut;
                }
            }
        }
        else
        {
            TRACE_DEBUG(">>>��ȡ�ϴβ��Խ����ʧ�ܣ������룺%d.\n\n",testResult);
            goto testOut;
        }
    }

    //3������---����EEPROM--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_ERASE_EEPROM;
        TRACE_DEBUG("---%d---����EEPROM-----------",testItems);
        RET_TEST_WRITEREAD_TESTRESULT ret_eraseTestResult = TestProcess_clearTestResult(test_process_state);
        checkResult[testItems] = ret_eraseTestResult;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>����EEPROM������ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>����EEPROM�����ʧ�ܣ������룺%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_ERASE_EEPROM_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //4������---����д��--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_SET_INFO;
        TRACE_DEBUG("---%d---����д��--------------",testItems);
        RET_TEST_SETINFO ret_setInfo = TestProcess_setInfo(targetModel);
        checkResult[testItems] = ret_setInfo;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>����д�룺�ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>����д�룺ʧ�ܣ������룺%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_SET_INFO_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //5������---����У��--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_SET_INFO;
        TRACE_DEBUG("---%d---����У��--------------",testItems);
        RET_TEST_SETINFO ret_setInfo = TestProcess_readInfo(targetModel);
        checkResult[testItems] = ret_setInfo;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>����У�飺�ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>����У�飺ʧ�ܣ������룺%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_CHECK_INFO_FAIL;
            GOTO_TESTEND
        }
    }
    //д�����ã���Ҫ��λ������Ŀ��岻����Ч����λĿ��
    TestProcess_resetTarget();
    testItems++;

    //6������---���Բ�------------------------------------------------------------------------
    if(processActive[testItems])
    {
        //�Բ⣺Ŀ����ϵ���Ҫ��Լ4��ʱ������Բ�
        test_process_state = TEST_PROCESS_STATE_SELFCHECK;
        TRACE_DEBUG("---%d---���Բ�----------------",testItems);
        RET_TEST_SELF ret_self = TestProcess_self(targetModel);
        if(RET_TEST_SELF_SUCCESS == ret_self)
        {
            memcpy(checkSelfResult, selfTestResult, LEN_SELF_TEST_RESULT);
            for(uint8_t i=0; i<LEN_SELF_TEST_RESULT; i++)
            {
                if(0x01 != checkSelfResult[i])
                {
                    switch(i)
                    {
                        case 0:
                        {
                            TRACE_DEBUG("\n�Բ�ʧ����Ŀ��MCU��BLE�Ĵ���ͨ�š�\n");
                            break;
                        }
                        case 1:
                        {
                            TRACE_DEBUG("\n�Բ�ʧ����Ŀ��MCU��zigbee�Ĵ���ͨ�š�\n");
                            break;
                        }
                        case 2:
                        {
                            TRACE_DEBUG("\n�Բ�ʧ����Ŀ��MCU��EEPROM�Ĵ���ͨ�š�\n");
                            break;
                        }
                        case 3:
                        {
                            TRACE_DEBUG("\n�Բ�ʧ����Ŀ��MCU��ָ��ģ��Ĵ���ͨ�š�\n");
                            break;
                        }
                        case 4:
                        {
                            TRACE_DEBUG("\n�Բ�ʧ����Ŀ��MCU�����FLASH�Ĵ���ͨ�š�\n");
                            break;
                        }
                        case 5:
                        {
                            TRACE_DEBUG("\n�Բ�ʧ����Ŀ��MCU��RTC�Ĵ���ͨ�š�\n");
                            break;
                        }
                        case 6:
                        {
                            TRACE_DEBUG("\n�Բ�ʧ����Ŀ��MCU��ADC�Ĳ��ԡ�\n");
                            break;
                        }
                    }
                    test_process_state = TEST_PROCESS_STATE_SELFCHECK_FAIL;
                }
            }
        }
        else
        {
            memset(checkSelfResult, 0x01, LEN_SELF_TEST_RESULT);
            test_process_state = TEST_PROCESS_STATE_SELFCHECK_FAIL;
            TRACE_DEBUG(">>>���Բ⣺ʧ�ܣ���ȡ���Բ���ʧ�ܣ�\n\n",);
        }
        if(TEST_PROCESS_STATE_SELFCHECK_FAIL == test_process_state)
        {
            test_process_state = TEST_PROCESS_STATE_SELFCHECK_FAIL;
            GOTO_TESTEND
        }
        else
        {
            TRACE_DEBUG(">>>���Բ⣺�ɹ�.\n\n");
        }
    }
    testItems ++;

    //8������---�����ת����--------------------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_MOTOR_FORWARD;
        TRACE_DEBUG("---%d---�����ת����-----------",testItems);
        RET_TEST_MOTOR ret_motor = TestProcess_motorForward();
        checkResult[testItems] = ret_motor;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>�����ת���ԣ��ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>�����ת���ԣ�ʧ�ܣ������룺%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_MOTOR_FORWARD_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //9������---�����ת����---------------------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_MOTOR_REVERSAL;
        TRACE_DEBUG("---%d---�����ת����-----------",testItems);
        RET_TEST_MOTOR ret_motor = TestProcess_motorReversal();
        checkResult[testItems] = ret_motor;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>�����ת���ԣ��ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>�����ת���ԣ�ʧ�ܣ������룺%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_MOTOR_REVERSAL_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //10������---������ť���--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_ANTI_LOCK;
        TRACE_DEBUG("---%d---������ť���---------",testItems);
        RET_TEST_SWITCH ret_antiLock = TestProcess_antiLock();
        checkResult[testItems] = ret_antiLock;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>������ť��⣺�ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>������ť��⣺ʧ�ܣ������룺%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_ANTI_LOCK_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //11������---���˿��ؼ��--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_PREVENT_DISMANTLE;
        TRACE_DEBUG("---%d---���˿��ؼ��----------",testItems);
        RET_TEST_SWITCH ret_preventDismantle = TestProcess_preventDismantle();
        checkResult[testItems] = ret_preventDismantle;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>���˿��ؼ�⣺�ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>���˿��ؼ�⣺ʧ�ܣ������룺%d.\n\n",
                        checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_PREVENT_DISMANTLE_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //12������---���ü�--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_SETTING_BUTTON;
        TRACE_DEBUG("---%d---���ü�---------------",testItems);
        RET_TEST_SWITCH ret_settingButton = TestProcess_settingButton();
        checkResult[testItems] = ret_settingButton;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>���ü����ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>���ü���ʧ�ܣ������룺%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_SETTING_BUTTON_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //13������---��ռ�--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_CLEAR_BUTTON;
        TRACE_DEBUG("---%d---��ռ�---------------",testItems);
        RET_TEST_SWITCH ret_clearButton = TestProcess_clearButton();
        checkResult[testItems] = ret_clearButton;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>��ռ����ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>��ռ���ʧ�ܣ������룺%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_CLEAR_BUTTON_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //14������---б��--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_OBLIQUE_TONGUE;
        TRACE_DEBUG("---%d---б��-----------------",testItems);
        RET_TEST_SWITCH ret_obliqueTonque = TestProcess_obliqueTongue();
        checkResult[testItems] = ret_obliqueTonque;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>б�ࣺ�ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>б�ࣺʧ�ܣ������룺%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_OBLIQUE_TONGUE_FAIL;
            GOTO_TESTEND
        }
    }
    testItems++;

    //15������---���߹��Ĳ���--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_POWER_MIN;
        TRACE_DEBUG("---%d---���߹��Ĳ���---------",testItems);
        RET_TEST_POWER_MODE ret_powerModeSleep = TestProcess_powerMode(POWER_MODE_SLEEP);
        checkResult[testItems] = ret_powerModeSleep;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>���߹��Ĳ��ԣ��ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>���߹��Ĳ��ԣ�ʧ�ܣ������룺%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_POWER_MAX_FAIL;
            GOTO_TESTEND
        }
        //��λĿ��-���ߺ��޷����ѣ�ֻ��������������̽���ȥ
//        if(processActive[testItems+1])
            TestProcess_resetTarget();
    }
    testItems++;

    //16������---ˢ������----------------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_CARD;
        TRACE_DEBUG("---%d---ˢ������-------------",testItems);
        RET_TEST_CARD ret_card = TestProcess_card();
        checkResult[testItems] = ret_card;
        if (0x00 == checkResult[testItems])
         {
             TRACE_DEBUG(">>>ˢ�����ԣ��ɹ�.\n\n");
         }
         else
         {
             TRACE_DEBUG(">>>ˢ�����ԣ�ʧ�ܣ������룺%d.\n\n", checkResult[testItems]);
             test_process_state = TEST_PROCESS_STATE_CARD_FAIL;
             //        GOTO_TESTEND
         }
    }
    testItems++;

    //17������---���̲���-------------------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_KEYBOARD;
        TRACE_DEBUG("---%d---���̲���-------------",testItems);
        RET_TEST_KEYBOARD ret_keyBoard = TestProcess_keyBoard();
        checkResult[testItems] = ret_keyBoard;
        if (0x00 == checkResult[testItems])
         {
             TRACE_DEBUG(">>>���̲��ԣ��ɹ�.\n\n");
         }
         else
         {
             TRACE_DEBUG(">>>���̲��ԣ�ʧ�ܣ������룺%d.\n\n", checkResult[testItems]);
             test_process_state = TEST_PROCESS_STATE_KEYBOARD_FAIL;
             //        GOTO_TESTEND
         }
    }
    testItems++;

    //18������---��������--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_VOICE;
        TRACE_DEBUG("---%d---��������-------------",testItems);
        RET_TEST_VOICE ret_voice = TestProcess_voice();
        checkResult[testItems] = ret_voice;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>�������ԣ��ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>�������ԣ�ʧ�ܣ������룺%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_VOICE_FAIL;
            //        GOTO_TESTEND
        }
    }
    testItems++;

    //19������---LED�Ʋ���--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_LED;
        TRACE_DEBUG("---%d---LED�Ʋ���------------",testItems);
        RET_TEST_LED ret_led = TestProcess_led();
        checkResult[testItems] = ret_led;
        if (0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>LED�Ʋ��ԣ��ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>LED�Ʋ��ԣ�ʧ�ܣ������룺%d.\n\n", checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_LED_FAIL;
            //        GOTO_TESTEND
        }
    }
    testItems++;

    //7������---BLEģ�����ֻ�ͨ�Ų���--------------------------------------------------------------------
    if(processActive[testItems])
    {
        test_process_state = TEST_PROCESS_STATE_BLE;
        TRACE_DEBUG("---%d---BLEͨ�Ų���-----------",testItems);
        RET_TEST_BLE ret_ble = TestProcess_ble(NULL);
        checkResult[testItems] = ret_ble;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>BLEͨ�Ų��ԣ��ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>BLEͨ�Ų��ԣ�ʧ�ܣ������룺%d.\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_BLE_FAIL;
            switch(ret_ble)
            {
                case RET_TEST_BLE_TIMEOUT:
                {
                    TRACE_DEBUG(">>>ʧ��ԭ��δ֪.\n\n",);
                    break;
                }
                case RET_TEST_BLE_DISCOVERTIMEOUT:
                {
                    TRACE_DEBUG(">>>ʧ��ԭ��������ʱ.\n\n",);
                    break;
                }
                case RET_TEST_BLE_NOFIND:
                {
                    TRACE_DEBUG(">>>ʧ��ԭ��δ�ҵ�BLE.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_MTUSIZE:
                {
                    TRACE_DEBUG(">>>ʧ��ԭ��MTU size.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_PROCEDURE_COMPLETE:
                {
                    TRACE_DEBUG(">>>ʧ��ԭ��δ֪.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_CHARACTERISTIC:
                {
                    TRACE_DEBUG(">>>ʧ��ԭ�򣺹ؼ��ֻ�ȡʧ��.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_CONNECT:
                {
                    TRACE_DEBUG(">>>ʧ��ԭ������ʧ��.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_DATATRANS:
                {
                    TRACE_DEBUG(">>>ʧ��ԭ�����ݴ���ʧ��.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_GETBLEINFO:
                {
                    TRACE_DEBUG(">>>ʧ��ԭ�򣺻�ȡBLE��Ϣʧ��.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_ACTIVEBLE:
                {
                    TRACE_DEBUG(">>>ʧ��ԭ������ģ�黽��ʧ��.\n\n",);
                    break;
                }
                case RET_TEST_BLE_ERROR_NOTINIT:
                {
                    TRACE_DEBUG(">>>ʧ��ԭ������ģ��δ��ʼ��.\n\n",);
                    break;
                }
            }
            GOTO_TESTEND
        }
    }
    testItems++;

    //�жϲ��Խ��
    TRACE_DEBUG("\n====================================");
    TRACE_DEBUG("\n========���Խ��=====================");
    TRACE_DEBUG("\n=====================================\n");
    uint8_t testRecord = TEST_RESULT_SUCCESS;
    for(uint8_t i = 0; i<TOTAL_TEST_ITEM; i++)
    {
        if( processSelfActive[i] && (0x00 != checkResult[i]) )
        {
            TRACE_CODE("���Խ��ʧ����Ŀ��%d��\n",i);
            testRecord = i;
            break;
        }
    }

    if(testRecord == TEST_RESULT_SUCCESS)
    {
        for(uint8_t i = 0; i<LEN_SELF_TEST_RESULT; i++)
        {
            if( processSelfActive[i] && (0x01 != checkSelfResult[i]) )
            {
                TRACE_CODE("�Բ���ʧ����Ŀ��%d��\n",i);
                testRecord = 0x40 + i;
                break;
            }
        }
    }

    if(testRecord == TEST_RESULT_SUCCESS)
        TRACE_DEBUG("�ܲ��Խ��Ϊ==================================================|>>>�ɹ�<<<|��\n");
    else
        TRACE_DEBUG("�ܲ��Խ��Ϊ**************************************************|ʧ�ܣ�ʧ����Ŀ��%d����\n",testRecord);

    //20������---д����Խ��--------------------------------------------------------------------
    testEnd:
    if(processActive[testItems])
    {
    //    test_process_state = TEST_PROCESS_STATE_WRITE_TESTRESULT;
        TRACE_DEBUG("---%d---д����Խ��---------",testItems);
        RET_TEST_WRITEREAD_TESTRESULT ret_writeTestResult = TestProcess_writeTestResult(testRecord);
        checkResult[testItems] = ret_writeTestResult;
        if(0x00 == checkResult[testItems])
        {
            TRACE_DEBUG(">>>д����Խ�����ɹ�.\n\n");
        }
        else
        {
            TRACE_DEBUG(">>>д����Խ����ʧ�ܣ������룺%d.\n\n",checkResult[testItems]);
    //        test_process_state = TEST_PROCESS_STATE_WRITE_TESTRESULT_FAIL;
    //        GOTO_TESTEND
        }
    }
    testItems++;

    //21������---��ȡ���Խ��--------------------------------------------------------------------
    if(processActive[testItems])
    {
    //    test_process_state = TEST_PROCESS_STATE_READ_TESTRESULT;
        TRACE_DEBUG("---%d---��ȡ���Խ��---------",testItems);
        char testResult;
        RET_TEST_WRITEREAD_TESTRESULT ret_readTestResult = TestProcess_readTestResult(&testResult);
        checkResult[testItems] = ret_readTestResult;
        if(0x00 == checkResult[testItems])
        {
            if(testResult != testRecord)
            {
                TRACE_DEBUG(">>>��ȡ���Խ�����Ա�ʧ�ܣ�%d\n\n",testResult);
                test_process_state = TEST_PROCESS_STATE_READ_TESTRESULT_FAIL;
            }
            else
            {
                TRACE_DEBUG(">>>��ȡ���Խ�����Աȳɹ�.\n\n");
            }
        }
        else
        {
            TRACE_DEBUG(">>>��ȡ���Խ����ʧ�ܣ������룺%d.\n\n",checkResult[testItems]);
            test_process_state = TEST_PROCESS_STATE_READ_TESTRESULT_FAIL;
    //        GOTO_TESTEND
        }
    }
    testItems++;

    //���Խ���---------------------------------------------------------------------------------
    testOut:
    test_process_state = TEST_PROCESS_STATE_IDLE;
    TRACE_DEBUG("---���Խ���---------\n");
    return test_process_state;
}

void Test_process_setMacAddr(uint8_t* addr)
{
    memcpy(macAddr, addr, 6);
}
