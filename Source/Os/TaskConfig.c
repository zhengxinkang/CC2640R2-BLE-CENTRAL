/*
 * TaskConfig.c
 *
 *  Created on: 2018��7��5��
 *      Author: zheng
 */

//#include <Driver_portWake.h>
#include "TaskConfig.h"
#include "Driver_UartEmulator.h"
#include "icall.h"
//#include "peripheral.h"
//#include "simple_peripheral.h"
#include "CmdLine.h"
#include "Driver_UartEmulator.h"
//#include "Driver_uart.h"
#include "Trace.h"
//#include "IwupUart_adapter.h"
//#include "Driver_portPa.h"
//#include "ProtocolBle.h"
//#include "Ble_timer.h"
#include "TimerConfig.h"
#include "ConfigProtocol.h"


void AllTask_create()
{
//    /* Start tasks of external images - Priority 5 */
//    ICall_createRemoteTasks();
//
//    /* Kick off profile - Priority 3 */
//    GAPRole_createTask();
//
//    /* Start task of SimpleBLEPeripheral */
//    SimpleBLEPeripheral_createTask();
}

void Module_initInTask()
{
    TimerConfig_init();
#if CONSOLE_EMULATOR
    Driver_uartEmulator_init(115200);
    CmdLine_Init();
#endif  //CONSOLE_EMULATOR
//    Driver_uartInit(115200);
//    Uart_adapter_init();
//
//    WakePort_init();
//#if PA_ENABLE
//    Driver_portPa_enable();
//#endif
//    Protocol_BLEInit();
}