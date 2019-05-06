/*
 * TaskConfig.h
 *
 *  Created on: 2018��7��5��
 *      Author: zheng
 */

#ifndef SOURCE_OS_TASKCONFIG_H_
#define SOURCE_OS_TASKCONFIG_H_


#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Queue.h>
/**********************************************/
/***********Task SimplePeripheral*************/
/*********************************************/
// Task configuration
#define SBP_TASK_PRIORITY               2
#define SBP_TASK_STACK_SIZE             3048


/**********************************************/
/***************Task Console******************/
/*********************************************/
#define CONSOLE_TASK_PRIORITY           1
#define CONSOLE_TASK_STACK_SIZE         2000



void AllTask_create();
void Module_initInTask();

#endif /* SOURCE_OS_TASKCONFIG_H_ */
