#ifndef EX_INCLUDE_TIRTOS_H
#define EX_INCLUDE_TIRTOS_H


#undef DEVICE_FAMILY_PATH
#define DEVICE_FAMILY_PATH(x) <ti/devices/DEVICE_FAMILY/x>


#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <Board.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>
#include DEVICE_FAMILY_PATH(driverlib/aux_adc.h)


// Handle renaming between SDK releases
#ifndef Board_RLED
    #ifdef Board_PIN_RLED
        #define Board_RLED      Board_PIN_RLED
    #else
        #define Board_RLED      Board_PIN_LED1
    #endif
#endif
#ifndef Board_GLED
    #ifdef Board_PIN_GLED
        #define Board_GLED      Board_PIN_GLED
    #else
        #define Board_GLED      Board_PIN_LED0
    #endif
#endif
#ifndef Board_LED_ON
    #define Board_LED_ON    Board_GPIO_LED_ON
#endif
#ifndef Board_LED_OFF
    #define Board_LED_OFF   Board_GPIO_LED_OFF
#endif
#ifndef Board_BTN1
    #define Board_BTN1      Board_PIN_BUTTON0
#endif
#ifndef Board_BTN2
    #define Board_BTN2      Board_PIN_BUTTON1
#endif
#ifndef Board_UART
    #define Board_UART      Board_UART0
#endif


#endif
