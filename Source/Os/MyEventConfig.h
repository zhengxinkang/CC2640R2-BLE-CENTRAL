/*
 * MyEventConfig.h
 *
 *  Created on: 2018Äê7ÔÂ7ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_OS_MYEVENTCONFIG_H_
#define SOURCE_OS_MYEVENTCONFIG_H_


#include "simple_central.h"
#include "Driver_UartEmulator.h"
#include "TestProcess_ble.h"
//#include "Driver_uart.h"
//#include "StateMachine_peripheral.h"
#include <ti/sysbios/knl/Event.h>


// Simple Central Task Events
#define SBC_ICALL_EVT                           ICALL_MSG_EVENT_ID // Event_Id_31
#define SBC_QUEUE_EVT                           UTIL_QUEUE_EVENT_ID // Event_Id_30
#define SBC_START_DISCOVERY_EVT                 Event_Id_00
#define MY_UARTEMULATRO_EVT                     Event_Id_01
#define MY_SENDDATA_EVT                         Event_Id_02
#define MY_TEST_PROCESS                         Event_Id_03

#define SBC_ALL_EVENTS                        (SBC_ICALL_EVT            | \
                                               SBC_QUEUE_EVT            | \
                                               MY_UARTEMULATRO_EVT      |\
                                               MY_SENDDATA_EVT          |\
                                               MY_TEST_PROCESS          |\
                                               SBC_START_DISCOVERY_EVT)


extern void MyEvent_post(UArg arg);
void        MyEventConfig_handle(uint32_t events);

extern void MyEvent_uartEmulatorHandle();
extern void MyEvent_sendDataHandle();
extern void MyEvent_testProcessHandle();


#endif /* SOURCE_OS_MYEVENTCONFIG_H_ */
