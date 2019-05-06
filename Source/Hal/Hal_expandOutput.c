/*
 * Hal_expandOutput.c
 *
 *  Created on: 2018Äê11ÔÂ23ÈÕ
 *      Author: zheng
 */
#include "Hal_expandOutput.h"
#include "Driver_hc595.h"

static uint32_t expandOutputData = 0x00000000;

void Hal_expandOutput_init()
{
    Driver_hc595_init();
    Driver_hc595_sendData(expandOutputData);
}

void Hal_expandOutput(EXPAND_OUTPUT_SERIAL serial, PortValue value)
{
    if(PORT_VALUE_LOW == value)
    {
        expandOutputData = expandOutputData&(~(0x00000001<<serial));
    }
    else if(PORT_VALUE_HIGH == value)
    {
        expandOutputData = expandOutputData|(0x00000001<<serial);
    }
    Driver_hc595_sendData(expandOutputData);
}
