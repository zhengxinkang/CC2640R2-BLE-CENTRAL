/*
 * Hal_abnormalRecord.h
 *
 *  Created on: 2019Äê5ÔÂ8ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_CURRENTMONITOR_HAL_ABNORMALRECORD_H_
#define SOURCE_TASKTEST_CURRENTMONITOR_HAL_ABNORMALRECORD_H_


void Hal_abnormalRecord_init();
uint32_t Hal_abnormalRecord_indexRead();
bool Hal_abnormalRecord_indexWrite();
void Hal_abnormal_recordText(uint8_t status_record, uint32_t* current, uint32_t currentAvg, uint32_t second);


#endif /* SOURCE_TASKTEST_CURRENTMONITOR_HAL_ABNORMALRECORD_H_ */
