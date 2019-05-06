/*
 * Driver_adc.c
 *
 *  Created on: 2018年9月11日
 *      Author: zheng
 */
#include "Driver_adc.h"
#include "board.h"
#include "Trace.h"

ADC_Handle   adc;
ADC_Params   params;

void        Driver_adcInit()
{
    ADC_init();
    ADC_Params_init(&params);
}

uint32_t    Driver_adcGet()
{
    int_fast16_t res;
    uint32_t adcValue0 = 0;
    adc = ADC_open(Board_ADC0, &params);
    /* Blocking mode conversion */
    res = ADC_convert(adc, &adcValue0);

    uint32_t adcValue0MicroVolt = ADC_convertRawToMicroVolts(adc, adcValue0);

    TRACE_DEBUG("电压采样结果-ADC channel 0: %d uV\n", adcValue0MicroVolt);

    ADC_close(adc);
    return adcValue0;
}
