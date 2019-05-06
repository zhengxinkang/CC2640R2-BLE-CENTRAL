/*
 * Driver_interrupt.c
 *
 *  Created on: 2018��11��23��
 *      Author: zheng
 */
#include "Trace.h"
#include "BF_Util.h"
#include "Hal_expandInput.h"
#include "Hal_interruptEvent.h"
#include "TaskTest.h"

#define TIMES_CHANGE_STATE      3

#define TIMER_INPUT_PERIOD      5
#define NUM_INPUT_PORT          24

static uint8_t inputTimeIndex;
static uint32_t inuptDataSave = 0x000000;
static uint32_t interruptBit = 0x000000;
static uint8_t stateChangeCount[NUM_INPUT_PORT];

static uint32_t sw_int1_pressCount_10ms = 0;
static uint32_t sw_int2_pressCount_10ms = 0;
static uint32_t sw_int3_pressCount_10ms = 0;


static void input_timeoutHandler();
static void Driver_interruptHandle(uint32_t event);
static void Driver_interrupt_keep(uint32_t events);

void Driver_interrupt_init()
{
    BF_UtilRegeistTimerCallback(input_timeoutHandler, &inputTimeIndex, TIMER_INPUT_PERIOD);
    BF_UtilWaitHandle(inputTimeIndex);
    inuptDataSave = Hal_expandInput_allGet();
    memset(stateChangeCount, 0x00, NUM_INPUT_PORT);
}

static void input_timeoutHandler()
{
    BF_UtilWaitHandle(inputTimeIndex);
    uint32_t inuptData = Hal_expandInput_allGet();
    Driver_interrupt_keep(inuptData);
    interruptBit = 0x00000000;
    for(uint8_t i = 0; i<NUM_INPUT_PORT; i++)
    {
        //������Щû���õ��Ķ˿�
        if(i!=0 && i!=1 && i!=2 && i!=3 && i!=4 && i!=14 && i!=15)
        {
            if( (inuptData & 0x00000001<<i)!=(inuptDataSave & 0x00000001<<i) )
            {
                stateChangeCount[i]++;
                if(stateChangeCount[i] >= TIMES_CHANGE_STATE)
                {
                    if(inuptData & 0x00000001<<i)
                    {
                        inuptDataSave = inuptDataSave|(0x00000001<<i);
                    }
                    else
                    {
                        inuptDataSave = inuptDataSave&(~(0x00000001<<i));
                    }
                    interruptBit = interruptBit|0x00000001<<i;
                }
            }
            else
            {
                stateChangeCount[i] = 0;
            }
        }
    }
    if(interruptBit)
    {
        //�����ж���Ϣ
        Driver_interruptHandle(interruptBit);
    }
}

static void Driver_interruptHandle(uint32_t event)
{
    for(EXPAND_INPUT_SERIAL i = KEY_IN1; i<NUM_INPUT_PORT; i++)
    {
        if(event & 0x00000001<<i)
        {
            //������Щû���õ��Ķ˿�
            if(i!=0 && i!=1 && i!=2 && i!=3 && i!=4 && i!=14 && i!=15)
            {
                TRACE_CODE("Interrupt event: port-%d, state-%d.\n", i, Hal_expandInput_get(i));
            }

            switch(i)
            {
                case KEY_IN1:
                {

                }
                break;
                case KEY_IN2:
                {

                }
                break;
                case KEY_IN3:
                {

                }
                break;
                case KEY_IN4:
                {

                }
                break;
                case KEY_IN5:
                {

                }
                break;
                case SW_IN1://��ť1
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(SW_IN1))
                    {
                        TRACE_CODE("���°���1.\n");
                        sw_int1_pressCount_10ms = 1;
                        SwInt1_pressDown();
                    }
                    else//����
                    {
                        TRACE_CODE("���𰴼�1.\n");
                        sw_int1_pressCount_10ms = 0;
                        SwInt1_preesUp();
                    }
                }
                break;
                case SW_IN2://��ť2
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(SW_IN2))
                    {
                        TRACE_CODE("���°���2.\n");
                        sw_int2_pressCount_10ms = 1;
                        SwInt2_preesDown();
                    }
                    else//����
                    {
                        TRACE_CODE("���𰴼�2.\n");
                        sw_int2_pressCount_10ms = 0;
                        SwInt2_preesUp();
                    }
                }
                break;
                case SW_IN3://��ť3
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(SW_IN3))
                    {
                        TRACE_CODE("���°���3.\n");
                        sw_int3_pressCount_10ms = 1;
                        SwInt3_preesDown();
                    }
                    else//����
                    {
                        TRACE_CODE("���𰴼�3.\n");
                        sw_int3_pressCount_10ms = 0;
                        SwInt3_preesUp();
                    }
                }
                break;
                case MOTOR_SW2:
                {
                    //�����λ
                    if(PORT_VALUE_LOW == Hal_expandInput_get(MOTOR_SW2))
                    {
                        TRACE_CODE("MOTOR_SW2 �����λ*.\n");
                        TestEvent_post(EVENT_TESTPROCESS_MOTOR_SW2);
                    }
                    else//����뿪
                    {
                        TRACE_CODE("MOTOR_SW2 ����뿪.\n");
                    }
                }
                break;
                case MOTOR_SW1:
                {
                    //�����λ
                    if(PORT_VALUE_LOW == Hal_expandInput_get(MOTOR_SW1))
                    {
                        TRACE_CODE("MOTOR_SW1 �����λ*.\n");
                        TestEvent_post(EVENT_TESTPROCESS_MOTOR_SW1);
                    }
                    else//����뿪
                    {
                        TRACE_CODE("MOTOR_SW1 ����뿪.\n");
                    }
                }
                break;
                case NULL_1:
                {

                }
                break;
                case NULL_2:
                {

                }
                break;
                case R_LED1:
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED1))
                    {
                        TRACE_CODE("R_LED1 ����*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//���
                    {
                        TRACE_CODE("R_LED1 ���.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED2:
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED2))
                    {
                        TRACE_CODE("R_LED2 ����*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//���
                    {
                        TRACE_CODE("R_LED2 ���.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED3:
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED3))
                    {
                        TRACE_CODE("R_LED3 ����*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//���
                    {
                        TRACE_CODE("R_LED3 ���.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED4:
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED4))
                    {
                        TRACE_CODE("R_LED4 ����*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//���
                    {
                        TRACE_CODE("R_LED4 ���.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED5:
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED5))
                    {
                        TRACE_CODE("R_LED5 ����*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//���
                    {
                        TRACE_CODE("R_LED5 ���.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED6:
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED6))
                    {
                        TRACE_CODE("R_LED6 ����*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//���
                    {
                        TRACE_CODE("R_LED6 ���.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED7:
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED7))
                    {
                        TRACE_CODE("R_LED7 ����*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//���
                    {
                        TRACE_CODE("R_LED7 ���.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED8:
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED8))
                    {
                        TRACE_CODE("R_LED8 ����*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//���
                    {
                        TRACE_CODE("R_LED8 ���.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED9:
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED9))
                    {
                        TRACE_CODE("R_LED9 ����*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//���
                    {
                        TRACE_CODE("R_LED9 ���.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED10:
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED10))
                    {
                        TRACE_CODE("R_LED10 ����*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//���
                    {
                        TRACE_CODE("R_LED10 ���.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED11:
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED11))
                    {
                        TRACE_CODE("R_LED11 ����*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//���
                    {
                        TRACE_CODE("R_LED11 ���.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED12:
                {
                    //����
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED1))
                    {
                        TRACE_CODE("R_LED12 ����*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//���
                    {
                        TRACE_CODE("R_LED12 ���.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;

                default:
                {
                    TRACE_ERROR("Driver_interruptHandle ERROR!\n");
                }
            }
        }
    }
}

static void Driver_interrupt_keep(uint32_t events)
{
    if(0 != sw_int1_pressCount_10ms)
    {
        sw_int1_pressCount_10ms++;
        if(sw_int1_pressCount_10ms == 2000/10)   //2000ms
        {
            TRACE_CODE("�������°���һ 2��.\n");
            SwInt1_press2S();
        }
    }
    if(0 != sw_int2_pressCount_10ms)
    {
        sw_int2_pressCount_10ms++;
        if(sw_int2_pressCount_10ms == 2000/10)   //2000ms
        {
            TRACE_CODE("�������°����� 2��.\n");
            SwInt2_press2S();
        }
    }
    if(0 != sw_int3_pressCount_10ms)
    {
        sw_int3_pressCount_10ms++;
        if(sw_int3_pressCount_10ms == 2000/10)   //2000ms
        {
            TRACE_CODE("�������°����� 2��.\n");
            SwInt3_press2S();
        }
    }
}
