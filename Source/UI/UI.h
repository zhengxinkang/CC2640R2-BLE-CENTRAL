/*
 * UI.h
 *
 *  Created on: 2018年12月17日
 *      Author: zheng
 */

#ifndef SOURCE_UI_UI_H_
#define SOURCE_UI_UI_H_


#include "stdint.h"
typedef enum{
    UI_TYPE_READY = 0,
    UI_TYPE_SUCCESS,
    UI_TYPE_FAIL,
    UI_TYPE_TESTED_PASS,    //已经测试过，上次测试成功
    UI_TYPE_TESTED_FAIL,    //已经测试过，上次测试失败
    UI_TYPE_QUIT,           //退出测试
    UI_TYPE_PROCESS,        //测试过程
}UI_TYPE;

void UI(UI_TYPE uiType, uint32_t data, uint8_t *str, uint32_t dataReason);


#endif /* SOURCE_UI_UI_H_ */
