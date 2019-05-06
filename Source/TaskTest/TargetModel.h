/*
 * TargetModel.h
 *
 *  Created on: 2018Äê9ÔÂ28ÈÕ
 *      Author: zheng
 */

#ifndef SOURCE_TASKTEST_TARGETMODEL_H_
#define SOURCE_TASKTEST_TARGETMODEL_H_


typedef enum{
    TARGET_MODEL_ERROR      = 0x00,
    TARGET_MODEL_S31        = 0x01,
    TARGET_MODEL_S31X       = 0x02,
    TARGET_MODEL_D3111      = 0x03,
    TARGET_MODEL_D3151      = 0x04,
    TARGET_MODEL_D3100      = 0x05,
    TARGET_MODEL_UNKNOW     = 0x06,
}TARGET_MODEL;

void TargetModel_init(TARGET_MODEL model);
TARGET_MODEL TargetModel_get();


#endif /* SOURCE_TASKTEST_TARGETMODEL_H_ */
