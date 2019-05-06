/*
 * MyEventConfig.c
 *
 *  Created on: 2018Äê8ÔÂ15ÈÕ
 *      Author: zheng
 */
#include "MyEventConfig.h"

void MyEventConfig_handle(uint32_t events)
{
    if (events & MY_UARTEMULATRO_EVT)
    {
        MyEvent_uartEmulatorHandle();
    }

    if (events & MY_SENDDATA_EVT)
    {
        MyEvent_sendDataHandle();
    }

    if (events & MY_TEST_PROCESS)
    {
        MyEvent_testProcessHandle();
    }

}
