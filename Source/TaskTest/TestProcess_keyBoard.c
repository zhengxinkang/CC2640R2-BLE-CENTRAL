/*
 * TestProcess_keyBoard.c
 *
 *  Created on: 2018年9月13日
 *      Author: zheng
 */
#include "TestProcess_keyBoard.h"
#include "String.h"
#include "TaskTest.h"
#include "Trace.h"
#include "stdbool.h"
#include "DataType.h"

#define NUM_KEY                         12
#define MAX_TIMES_KEY_TEST              50
#define TIMEOUT_TEST_PROCESS_KEYBOARD   1000000//1000秒

void TestProcess_showKeyBoard();

uint8_t keyTable[NUM_KEY]={0,0,0,0,0,0,0,0,0,0,0,0};

RET_TEST_KEYBOARD TestProcess_keyBoard()
{
    RET_TEST_KEYBOARD ret=RET_TEST_KEYBOARD_SUCCESS;

    bool keyFinish = false;
    memset(keyTable, 0, NUM_KEY);
    for(uint8_t i=0; i<MAX_TIMES_KEY_TEST; i++)
    {
        TestProcess_showKeyBoard();
        //等待键盘消息10秒钟
        uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_KEYBOARD, TIMEOUT_TEST_PROCESS_KEYBOARD);
        if(EVENT_TESTPROCESS_KEYBOARD == events)
        {
            uint8_t keySerial = 0xff;
            //获取键盘键号
            keySerial =keyValue;
            if(0xff != keySerial)
                keyTable[keySerial]=1;
        }
        //判断是否完成键盘检测
        keyFinish = true;
        for(uint8_t times = 0;times < NUM_KEY;times++)
        {
            if(0 == keyTable[times])
                keyFinish = false;
        }
        if(keyFinish)
        {
            ret=RET_TEST_KEYBOARD_SUCCESS;
            break;
        }
    }
    return ret;
}

void TestProcess_showKeyBoard()
{
    TRACE_DEBUG("还剩下这些按键未按下:\n");
    {
        TRACE_DEBUG("\n\t");
        if (!keyTable[1])
        TRACE_DEBUG("%d", 1);
        TRACE_DEBUG("\t");
        if (!keyTable[2])
        TRACE_DEBUG("%d", 2);
        TRACE_DEBUG("\t");
        if (!keyTable[3])
        TRACE_DEBUG("%d", 3);

        TRACE_DEBUG("\n\t");
        if (!keyTable[4])
        TRACE_DEBUG("%d", 4);
        TRACE_DEBUG("\t");
        if (!keyTable[5])
        TRACE_DEBUG("%d", 5);
        TRACE_DEBUG("\t");
        if (!keyTable[6])
        TRACE_DEBUG("%d", 6);

        TRACE_DEBUG("\n\t");
        if (!keyTable[7])
        TRACE_DEBUG("%d", 7);
        TRACE_DEBUG("\t");
        if (!keyTable[8])
        TRACE_DEBUG("%d", 8);
        TRACE_DEBUG("\t");
        if (!keyTable[9])
        TRACE_DEBUG("%d", 9);

        TRACE_DEBUG("\n\t");
        if (!keyTable[11])
        TRACE_DEBUG("*", 1);
        TRACE_DEBUG("\t");
        if (!keyTable[0])
        TRACE_DEBUG("%d", 0);
        TRACE_DEBUG("\t");
        if (!keyTable[10])
        TRACE_DEBUG("#", 1);
        TRACE_DEBUG("\n");
    }
}
