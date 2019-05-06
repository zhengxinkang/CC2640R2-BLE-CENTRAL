/*
 * TaskTest.h
 *
 *  Created on: 2018Äê8ÔÂ30ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TASKTEST_H_
#define SOURCE_TASKTEST_TASKTEST_H_


#include <ti/sysbios/knl/Event.h>

//event define
#define EVENT_START_TEST                        Event_Id_00

#define EVENT_TESTPROCESS_BLE                   Event_Id_01
#define EVENT_TESTPROCESS_CARD                  Event_Id_02
#define EVENT_TESTPROCESS_KEYBOARD              Event_Id_03
#define EVENT_TESTPROCESS_BLE_DISCOVER          Event_Id_04
#define EVENT_TESTPROCESS_BLE_DISCOVER_FAIL     Event_Id_05
#define EVENT_TESTPROCESS_BLE_CONNECT           Event_Id_06
#define EVENT_TESTPROCESS_BLE_DATA_TRANS        Event_Id_07
#define EVENT_TESTPROCESS_BLE_DISCONNECT        Event_Id_08
#define EVENT_TESTPROCESS_UART_ECHO             Event_Id_09
#define EVENT_TESTPROCESS_SET_DEVICEINFO        Event_Id_10
#define EVENT_TESTPROCESS_GET_DEVICEINFO        Event_Id_11
#define EVENT_TESTPROCESS_SELF                  Event_Id_12
#define EVENT_TESTPROCESS_GET_DEVICESTATUS      Event_Id_13
#define EVENT_TESTPROCESS_CONFIRM_SUCCESS       Event_Id_14
#define EVENT_TESTPROCESS_CONFIRM_FAIL          Event_Id_15
#define EVENT_TESTPROCESS_GET_BLEINFO           Event_Id_16
#define EVENT_TESTPROCESS_ACTION_ACTIVEBLE      Event_Id_17
#define EVENT_TESTPROCESS_WRITEREAD_TESTRESULT  Event_Id_18
#define EVENT_TESTPROCESS_EVENT_SETTINGBUTTON   Event_Id_19
#define EVENT_TESTPROCESS_EVENT_CLEARBUTTON     Event_Id_20
#define EVENT_TESTPROCESS_EVENT_OBLIQUETONGUE   Event_Id_21

#define EVENT_TESTPROCESS_MOTOR_SW1             Event_Id_22
#define EVENT_TESTPROCESS_MOTOR_SW2             Event_Id_23

#define EVENT_ADD_CARD                          Event_Id_24
#define EVENT_TRY_PW                            Event_Id_25

#define EVENT_TESTPROCESS_EVENT_VOICE           Event_Id_26
#define EVENT_LIGHT_ON                          Event_Id_27
#define EVENT_LIGHT_OFF                         Event_Id_28
#define EVENT_TESTPROCESS_BLE_DISCOVER_FAIL_RSSI     Event_Id_29
//#define EVENT_DO_UNLOCK                         Event_Id_01
//#define EVENT_BOLT_LOCK                         Event_Id_02
//#define EVENT_BOLT_UNLOCK                       Event_Id_03
//#define EVENT_DOOR_CLOSE                        Event_Id_04
//#define EVENT_DOOR_OPEN                         Event_Id_05
//#define EVENT_ABNORMAL_LOCK                     Event_Id_06
//#define EVENT_ABNORMAL_UNLOCK                   Event_Id_07
////event add
//#define TEST_ALL_EVENTS                        (EVENT_DO_UNLOCK         | \
//                                                EVENT_BOLT_LOCK         | \
//                                                EVENT_BOLT_UNLOCK       | \
//                                                EVENT_DOOR_CLOSE        | \
//                                                EVENT_DOOR_OPEN         | \
//                                                EVENT_ABNORMAL_LOCK     | \
//                                                EVENT_ABNORMAL_UNLOCK)

void        Test_createTask();
uint32_t    TestEvent_pend(uint32_t orMask, uint32_t timeout_ms);
void        TestEvent_post(uint32_t eventIds);


#endif /* SOURCE_TASKTEST_TASKTEST_H_ */
