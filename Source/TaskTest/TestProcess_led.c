/*
 * TestProcess_led.c
 *
 *  Created on: 2018��9��28��
 *      Author: zheng
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TestProcess_led.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "stdbool.h"
#include "DataType.h"
#include "TaskTest.h"
#include "Trace.h"
#include "BF_Util.h"
#include "Hal_expandInput.h"
#include "Hal_keyBoardLed.h"

#define MAX_TIMES_LED_CHECK             3
#define TIMES_LED_CHECK                 2
#define TIMEOUT_TEST_PROCESS_LED        500//1��

RET_TEST_LED TestProcess_led()
{
    uint8_t count_ledOn_check = 0;
    uint8_t count_ledOff_check = 0;
    RET_TEST_LED ret = RET_TEST_LED_SUCCESS;

//    TRACE_DEBUG("<<<<<<�ȴ�led���Խ��...\n��������led��˸�밴�³ɹ����������밴��ʧ�ܼ�\n");

    //�ȴ�������Ϣ10����
    for(uint8_t i = 0; i<MAX_TIMES_LED_CHECK; i++)
    {
        Task_sleep((400*1000)/Clock_tickPeriod);
        //����֮ǰ���¼�
        TestEvent_pend(EVENT_LIGHT_ON, 1);

        //����led����
        uint8_t para = ACTION_TYPE_LED;
        SendCmd_qcTest_action(&para, 1);

        uint32_t events = TestEvent_pend(EVENT_LIGHT_ON, TIMEOUT_TEST_PROCESS_LED);
        if (events & EVENT_LIGHT_ON)
        {
            bool isAllLightOn =(\
                IsLightOn(R_LED1) && IsLightOn(R_LED2) && IsLightOn(R_LED3) &&\
                IsLightOn(R_LED4) && IsLightOn(R_LED5) && IsLightOn(R_LED6) &&\
                IsLightOn(R_LED7) && IsLightOn(R_LED8) && IsLightOn(R_LED9) &&\
                IsLightOn(R_LED10) && IsLightOn(R_LED11) && IsLightOn(R_LED12)\
            );

            if(isAllLightOn)
            {
                ret = RET_TEST_LED_SUCCESS;
                TRACE_CODE("led onȷ��Ϊ���ɹ���.\n");
                count_ledOn_check++;
            }
            else
            {
                TRACE_CODE("led�Ƽ��ʧ��!\n",TIMEOUT_TEST_PROCESS_LED);
                for(uint8_t i=0;i<12;i++)
                {
                    if(!IsLightOn(LedSerialToCode(i)))
                    {
                        TRACE_CODE("����'%c'������\n", LedSerialToChar(i));
                        ret = i + 1;
                    }
                }
            }
        }
        else
        {
            ret = RET_TEST_LED_ERROR_MESG;
            TRACE_CODE("led on��Ϣ���ճ�ʱ!\n",TIMEOUT_TEST_PROCESS_LED);
        }

        if(RET_TEST_LED_SUCCESS == ret)
        {
            //����֮ǰ�¼�
            TestEvent_pend(EVENT_LIGHT_OFF,1);
            //�ȴ�����������¼�
            uint32_t events = TestEvent_pend(EVENT_LIGHT_OFF,TIMEOUT_TEST_PROCESS_LED);
            if (events & EVENT_LIGHT_OFF)
            {
                bool isAllLightOff = (!IsLightOn(R_LED1) && !IsLightOn(R_LED2)
                        && !IsLightOn(R_LED3) && !IsLightOn(R_LED4)
                        && !IsLightOn(R_LED5) && !IsLightOn(R_LED6)
                        && !IsLightOn(R_LED7) && !IsLightOn(R_LED8)
                        && !IsLightOn(R_LED9) && !IsLightOn(R_LED10)
                        && !IsLightOn(R_LED11) && !IsLightOn(R_LED12)\
                );

                if (isAllLightOff)
                {
                    TRACE_CODE("led offȷ��Ϊ���ɹ���.\n");
                    count_ledOff_check++;
                }
                else
                {
                    TRACE_CODE("led����Ƽ��ʧ��!\n", TIMEOUT_TEST_PROCESS_LED);
                    for (uint8_t i = 0; i < 12; i++)
                    {
                        if (IsLightOn(LedSerialToCode(i)))
                        {
                            TRACE_CODE("����'%c'����\n", LedSerialToChar(i));
                            ret = i + 1;
                        }
                    }
                }
            }
            else
            {
                ret = RET_TEST_LED_ERROR_MESG;
                TRACE_CODE("led off��Ϣ���ճ�ʱ!\n", TIMEOUT_TEST_PROCESS_LED);
            }
        }

        if(count_ledOn_check >= TIMES_LED_CHECK && count_ledOff_check >= TIMES_LED_CHECK)
        {
            ret = RET_TEST_LED_SUCCESS;
            break;
        }
        else if(i >= MAX_TIMES_LED_CHECK-1)
        {
//            ret = RET_TEST_LED_ERROR;
        }
    }
    return ret;
}



