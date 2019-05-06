/*
 * Hal_expandInput.c
 *
 *  Created on: 2018Äê11ÔÂ23ÈÕ
 *      Author: zheng
 */
#include "Hal_expandInput.h"
#include "Driver_hc165.h"

PortValue Hal_expandInput_get(EXPAND_INPUT_SERIAL serial)
{
    PortValue portValue = PORT_VALUE_LOW;
    uint32_t inuptData = Driver_hc165_get();

    if(inuptData & (0x00000001<<serial))
    {
        portValue = PORT_VALUE_HIGH;
    }
    else
    {
        portValue = PORT_VALUE_LOW;
    }
    return portValue;
}

uint32_t Hal_expandInput_allGet()
{
    return Driver_hc165_get();
}

bool IsLightOn(EXPAND_INPUT_SERIAL serial)
{
    return (PORT_VALUE_LOW == Hal_expandInput_get(serial));
}
