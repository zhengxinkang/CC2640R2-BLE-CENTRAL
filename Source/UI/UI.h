/*
 * UI.h
 *
 *  Created on: 2018��12��17��
 *      Author: zheng
 */

#ifndef SOURCE_UI_UI_H_
#define SOURCE_UI_UI_H_


#include "stdint.h"
typedef enum{
    UI_TYPE_READY = 0,
    UI_TYPE_SUCCESS,
    UI_TYPE_FAIL,
    UI_TYPE_TESTED_PASS,    //�Ѿ����Թ����ϴβ��Գɹ�
    UI_TYPE_TESTED_FAIL,    //�Ѿ����Թ����ϴβ���ʧ��
    UI_TYPE_QUIT,           //�˳�����
    UI_TYPE_PROCESS,        //���Թ���
}UI_TYPE;

void UI(UI_TYPE uiType, uint32_t data, uint8_t *str, uint32_t dataReason);


#endif /* SOURCE_UI_UI_H_ */
