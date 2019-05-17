/*
 * TaskConfig.c
 *
 *  Created on: 2018??7??5??
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
#include "Driver_adc.h"
#include "Driver_gpio.h"
#include "Driver_uart.h"
#include "TestProcess_ble.h"
#include "IwupUart_adapter.h"
//#include "Driver_uart.h"
#include "Trace.h"
//#include "IwupUart_adapter.h"
//#include "Driver_portPa.h"
//#include "ProtocolBle.h"
//#include "Ble_timer.h"
#include "TimerConfig.h"
#include "ConfigProtocol.h"
#include "TargetModel.h"
#include "Driver_portUart.h"
#include "Hal_expandOutput.h"
#include "Driver_ADCmpc3901.h"
#include "Hal_electricCurrent.h"
#include "Driver_CS1231.h"
#include "Driver_interrupt.h"
#include "Hal_oled.h"
#include "Lock_atcion.h"
#include "Hal_buzz.h"
#include "Hal_led.h"
#include "UTC_clock.h"
#include "Hal_flash.h"
#include "Driver_spi.h"
#include "Hal_oledDisplay.h"


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
    Driver_adcInit();
    Driver_gpioInit();
    UTC_init();

    TestProcess_bleInit();
    //老化治具，先关闭串口
    Driver_uart_init(9600, Uart_adapter_recieve);
    Driver_uartGpio_close();
    Driver_uart_open();
    Hal_expandOutput_init();
    Init_mcp3901(GAIN_NUM_ADC, 1);
    Hal_electricCurrent_init();
//    Init_adcIC();
    Driver_interrupt_init();
//    Hal_oled_init();
    Hal_oledDisplay_init();
    Lock_action_init();
    Buzz_init();
//    Buzz_action(200, 100, 1);
    Led_init();
    Hal_electricCurrent_offsetRead();
//    Driver_uart_open();
//    Driver_uartInit(115200);
//    Uart_adapter_init();
//
//    WakePort_init();
//#if PA_ENABLE
//    Driver_portPa_enable();
//#endif
//    Protocol_BLEInit();
#if defined     (TEST_D3151)
    #warning "TEST_D3151"
    TargetModel_init(TARGET_MODEL_S31X);
#elif defined   (TEST_D3111)
    #warning "TEST_D3111"
    TargetModel_init(TARGET_MODEL_D3111);
#elif defined   (TEST_D3100)
    #warning "TEST_D3100"
    TargetModel_init(TARGET_MODEL_D3100);
#else
    #error "TEST TARGET ERROR!"
#endif

#ifdef TEST_CURRENT_MONITOR
#include "Hal_abnormalRecord.h"
    extFlash_open();
    TRACE_DEBUG("FLASH ID IS : %x",extFlash_readId());
    UTC_setClock(Hal_flash_readTime());
    Hal_oled_abnormalCount(Hal_abnormalRecord_indexRead());
    Hal_oledDisplay_str(1,0,"[ready]  ");
#endif
}
