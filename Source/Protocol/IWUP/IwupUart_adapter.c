/*
 * Uart_adapter.c
 *
 *  Created on: 2017Äê11ÔÂ1ÈÕ
 *      Author: Administrator
 */
#include <Driver_uart.h>
#include "IWUP_config.h"
#include "IWUP_frame.h"

void Uart_adapter_send(uint8_t *buf, uint8_t len, uint8_t portNum)
{
    if(PORT_1 == portNum)
        Driver_uart_send(buf, len);
//    else if(PORT_2 == portNum)
//        Driver_uart_send2(buf, len);
}

void Uart_adapter_recieve(uint8_t* data, uint16_t len)
{
    IWUP_frame_recieve(data, len);
}
