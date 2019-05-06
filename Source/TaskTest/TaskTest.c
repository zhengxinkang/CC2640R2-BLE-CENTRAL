/*
 * TaskTest.c
 *
 *  Created on: 2018年8月30日
 *      Author: zheng
 */
#include "TaskConfig.h"
#include "TaskTest.h"
#include <string.h>
#include <stdbool.h>
#include "Trace.h"
#include "icall.h"
#include "Test_process.h"
#include "TestProcess_ble.h"

#define TEST_TASK_STACK_SIZE    1024
#define TEST_TASK_PRIORITY      2

Event_Handle taskTestEvent;

Task_Struct testTask;
char testTaskStack[TEST_TASK_STACK_SIZE]={1};


static void Test_taskFxn(UArg a0, UArg a1);
static void TaskTest_init();
//static void stepMotorTimer_handle(UArg arg);

void Test_createTask()
{
    Task_Params taskParams;

    // Configure task
    Task_Params_init(&taskParams);
    taskParams.stack = testTaskStack;
    taskParams.stackSize = TEST_TASK_STACK_SIZE;
    taskParams.priority = TEST_TASK_PRIORITY;

    Task_construct(&testTask, Test_taskFxn, &taskParams, NULL);
}

static void Test_taskFxn(UArg a0, UArg a1)
{
    TaskTest_init();

    for(;;)
    {
        uint32_t events;
        events = TestEvent_pend(EVENT_START_TEST, ICALL_TIMEOUT_FOREVER);

        if(events)
        {
            if (events & EVENT_START_TEST)
            {
                TRACE_DEBUG("Event start test.\n");
                Test_process();
            }
        }
    }
}

static void TaskTest_init()
{
    taskTestEvent = Event_create(NULL, NULL);
    if (taskTestEvent == NULL)
    {
        TRACE_FATAL("testTask.c:taskTestEvent create failed!\n");
    }
}

uint32_t TestEvent_pend(uint32_t orMask, uint32_t timeout_ms)
{
    uint32_t events;
    events = Event_pend(taskTestEvent, Event_Id_NONE, orMask, 100*timeout_ms);
    return events;
}

void TestEvent_post(uint32_t eventIds)
{
    Event_post(taskTestEvent, eventIds);
}