/*
 * Driver_ADCmpc3901.h
 *
 *  Created on: 2018年11月14日
 *      Author: zheng
 */

#ifndef SOURCE_DRIVER_DRIVER_ADCMPC3901_H_
#define SOURCE_DRIVER_DRIVER_ADCMPC3901_H_


#include "Driver_simulateSpi.h"
#define GAIN_NUM_ADC 2
void Init_mcp3901(uchar Pga_sel0, uchar Pga_sel1); //初始化MCP3901
int32_t Read_1CH_adc(uchar ch);


#endif /* SOURCE_DRIVER_DRIVER_ADCMPC3901_H_ */
