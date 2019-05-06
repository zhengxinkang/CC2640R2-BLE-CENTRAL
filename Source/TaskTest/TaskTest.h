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
