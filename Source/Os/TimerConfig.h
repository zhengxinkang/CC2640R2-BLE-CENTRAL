/*
 * TimerConfig.h
 *
 *  Created on: 2018Äê8ÔÂ15ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_OS_TIMERCONFIG_H_
#define SOURCE_OS_TIMERCONFIG_H_


#include <Util.h>
//#include "Ble_timer.h"


// How often to perform periodic event (in msec)
#define TIMER_SENDDATA_PERIOD_DEFAULT               200


extern Clock_Struct sendDataClock;


extern void TimerConfig_init();
extern void TimerConfig_start(Clock_Struct *pClock, uint32_t timeout);
extern void TimerConfig_stop(Clock_Struct *pClock);


#endif /* SOURCE_OS_TIMERCONFIG_H_ */
