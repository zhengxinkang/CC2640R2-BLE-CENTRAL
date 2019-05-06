/*
 * Driver_interrupt.c
 *
 *  Created on: 2018年11月23日
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
        //屏蔽这些没有用到的端口
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
        //发送中断消息
        Driver_interruptHandle(interruptBit);
    }
}

static void Driver_interruptHandle(uint32_t event)
{
    for(EXPAND_INPUT_SERIAL i = KEY_IN1; i<NUM_INPUT_PORT; i++)
    {
        if(event & 0x00000001<<i)
        {
            //屏蔽这些没有用到的端口
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
                case SW_IN1://按钮1
                {
                    //按下
                    if(PORT_VALUE_LOW == Hal_expandInput_get(SW_IN1))
                    {
                        TRACE_CODE("按下按键1.\n");
                        sw_int1_pressCount_10ms = 1;
                        SwInt1_pressDown();
                    }
                    else//弹起
                    {
                        TRACE_CODE("弹起按键1.\n");
                        sw_int1_pressCount_10ms = 0;
                        SwInt1_preesUp();
                    }
                }
                break;
                case SW_IN2://按钮2
                {
                    //按下
                    if(PORT_VALUE_LOW == Hal_expandInput_get(SW_IN2))
                    {
                        TRACE_CODE("按下按键2.\n");
                        sw_int2_pressCount_10ms = 1;
                        SwInt2_preesDown();
                    }
                    else//弹起
                    {
                        TRACE_CODE("弹起按键2.\n");
                        sw_int2_pressCount_10ms = 0;
                        SwInt2_preesUp();
                    }
                }
                break;
                case SW_IN3://按钮3
                {
                    //按下
                    if(PORT_VALUE_LOW == Hal_expandInput_get(SW_IN3))
                    {
                        TRACE_CODE("按下按键3.\n");
                        sw_int3_pressCount_10ms = 1;
                        SwInt3_preesDown();
                    }
                    else//弹起
                    {
                        TRACE_CODE("弹起按键3.\n");
                        sw_int3_pressCount_10ms = 0;
                        SwInt3_preesUp();
                    }
                }
                break;
                case MOTOR_SW2:
                {
                    //电机到位
                    if(PORT_VALUE_LOW == Hal_expandInput_get(MOTOR_SW2))
                    {
                        TRACE_CODE("MOTOR_SW2 电机到位*.\n");
                        TestEvent_post(EVENT_TESTPROCESS_MOTOR_SW2);
                    }
                    else//电机离开
                    {
                        TRACE_CODE("MOTOR_SW2 电机离开.\n");
                    }
                }
                break;
                case MOTOR_SW1:
                {
                    //电机到位
                    if(PORT_VALUE_LOW == Hal_expandInput_get(MOTOR_SW1))
                    {
                        TRACE_CODE("MOTOR_SW1 电机到位*.\n");
                        TestEvent_post(EVENT_TESTPROCESS_MOTOR_SW1);
                    }
                    else//电机离开
                    {
                        TRACE_CODE("MOTOR_SW1 电机离开.\n");
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
                    //亮起
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED1))
                    {
                        TRACE_CODE("R_LED1 亮起*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//灭掉
                    {
                        TRACE_CODE("R_LED1 灭掉.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED2:
                {
                    //亮起
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED2))
                    {
                        TRACE_CODE("R_LED2 亮起*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//灭掉
                    {
                        TRACE_CODE("R_LED2 灭掉.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED3:
                {
                    //亮起
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED3))
                    {
                        TRACE_CODE("R_LED3 亮起*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//灭掉
                    {
                        TRACE_CODE("R_LED3 灭掉.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED4:
                {
                    //亮起
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED4))
                    {
                        TRACE_CODE("R_LED4 亮起*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//灭掉
                    {
                        TRACE_CODE("R_LED4 灭掉.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED5:
                {
                    //亮起
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED5))
                    {
                        TRACE_CODE("R_LED5 亮起*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//灭掉
                    {
                        TRACE_CODE("R_LED5 灭掉.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED6:
                {
                    //亮起
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED6))
                    {
                        TRACE_CODE("R_LED6 亮起*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//灭掉
                    {
                        TRACE_CODE("R_LED6 灭掉.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED7:
                {
                    //亮起
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED7))
                    {
                        TRACE_CODE("R_LED7 亮起*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//灭掉
                    {
                        TRACE_CODE("R_LED7 灭掉.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED8:
                {
                    //亮起
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED8))
                    {
                        TRACE_CODE("R_LED8 亮起*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//灭掉
                    {
                        TRACE_CODE("R_LED8 灭掉.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED9:
                {
                    //亮起
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED9))
                    {
                        TRACE_CODE("R_LED9 亮起*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//灭掉
                    {
                        TRACE_CODE("R_LED9 灭掉.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED10:
                {
                    //亮起
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED10))
                    {
                        TRACE_CODE("R_LED10 亮起*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//灭掉
                    {
                        TRACE_CODE("R_LED10 灭掉.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED11:
                {
                    //亮起
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED11))
                    {
                        TRACE_CODE("R_LED11 亮起*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//灭掉
                    {
                        TRACE_CODE("R_LED11 灭掉.\n");
                        TestEvent_post(EVENT_LIGHT_OFF);
                    }
                }
                break;
                case R_LED12:
                {
                    //亮起
                    if(PORT_VALUE_LOW == Hal_expandInput_get(R_LED1))
                    {
                        TRACE_CODE("R_LED12 亮起*.\n");
                        TestEvent_post(EVENT_LIGHT_ON);
                    }
                    else//灭掉
                    {
                        TRACE_CODE("R_LED12 灭掉.\n");
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
            TRACE_CODE("持续按下按键一 2秒.\n");
            SwInt1_press2S();
        }
    }
    if(0 != sw_int2_pressCount_10ms)
    {
        sw_int2_pressCount_10ms++;
        if(sw_int2_pressCount_10ms == 2000/10)   //2000ms
        {
            TRACE_CODE("持续按下按键二 2秒.\n");
            SwInt2_press2S();
        }
    }
    if(0 != sw_int3_pressCount_10ms)
    {
        sw_int3_pressCount_10ms++;
        if(sw_int3_pressCount_10ms == 2000/10)   //2000ms
        {
            TRACE_CODE("持续按下按键三 2秒.\n");
            SwInt3_press2S();
        }
    }
}
