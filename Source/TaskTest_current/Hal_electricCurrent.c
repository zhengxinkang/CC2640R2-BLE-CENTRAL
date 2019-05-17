/*
 * Hal_electricCurrent.c
 *
 *  Created on: 2018��11��16��
 *      Author: zheng
 */
#include "BF_Util.h"
#include "Driver_ADCmpc3901.h"
#include "Trace.h"
#include "Hal_electricCurrent.h"
#include "Hal_oled.h"
#include "testCurrentConfig.h"
#include "TaskTest.h"
#include "Hal_buzz.h"
#include "Test_process.h"
#include "UTC_clock.h"
#include "Hal_flash.h"
#include "Hal_led.h"

#define TIMER_CURRENT_PERIOD    10
static uint8_t currentTimeIndex;

static void current_timeoutHandler();
void Hal_electricCurrent_judge(int32_t current);
void Hal_abnormal_judge(int32_t currentAvgr, int32_t currentThist);

void Hal_electricCurrent_init()
{
    BF_UtilRegeistTimerCallback(current_timeoutHandler, &currentTimeIndex, TIMER_CURRENT_PERIOD);
    BF_UtilWaitHandle(currentTimeIndex);
}

static int32_t offset = 0;
void Hal_electricCurrent_offsetRead()
{
    offset = Hal_flash_readCurrentOffset();
}

uint32_t Hal_electricCurrent_offsetGet()
{
    return offset;
}

#define AVG_TIMES_COUNT         8
static int32_t currentAvg[AVG_TIMES_COUNT]={0,0,0,0,0,0,0,0};
static bool isCurrentAvgActive = false;

#define CHECK_TIMES 50
static int32_t current[CHECK_TIMES];
static uint16_t currentIndex = 0;

/*
currentThis:  Ҫ���µı��ε���
isShift:      �����Ƿ���Ҫ����
times:        ��Ҫͳ�Ƽ��ε���ֵ
 */
int32_t avgCurrentCount(int32_t currentThis, bool isShift, uint8_t times)
{
    if(times > AVG_TIMES_COUNT)
        times = AVG_TIMES_COUNT;
    if(isShift)
    {
        if(!isCurrentAvgActive)
        {
            for(uint8_t i=0; i<AVG_TIMES_COUNT; i++)
            {
                currentAvg[i] = currentThis;
            }
            isCurrentAvgActive = true;
        }

        for(uint8_t i=0; i<AVG_TIMES_COUNT; i++)
        {
            if(AVG_TIMES_COUNT-1 == i)
            {
                currentAvg[AVG_TIMES_COUNT-1-i] = currentThis;
            }
            else
            {
                currentAvg[AVG_TIMES_COUNT-1-i] = currentAvg[AVG_TIMES_COUNT-2-i];
            }
        }
    }
    else
    {

    }

    //����ƽ��ֵ
    int32_t total = 0;
    for(uint8_t i=0; i<times; i++)
    {
        total += currentAvg[i];
    }
    return total/times;
}

static void current_timeoutHandler()
{
    BF_UtilWaitHandle(currentTimeIndex);
    current[currentIndex++]  = Read_1CH_adc(0) -Hal_electricCurrent_offsetGet();
    if(current[currentIndex-1]<0)
        current[currentIndex-1] = 1;

    if(currentIndex == CHECK_TIMES)
    {
        currentIndex = 0;
        int32_t currentTotal = 0;
        uint8_t currentCount = 0;
        for(uint8_t i = 0;i<CHECK_TIMES;i++)
        {
            TRACE_CODE("��%02d������С%d uA��\n",i, current[i]);

            if(current[i] > 0)
            {
                currentTotal += current[i];
                currentCount++;
            }
            else
            {
                current[i] = 0;
            }
        }
        int32_t currentAverage1 = currentTotal/currentCount;

        currentTotal = 0;
        currentCount = 0;
        int32_t astringent = 100;
        for(uint8_t i = 0;i<CHECK_TIMES;i++)
        {
            if(current[i]/currentAverage1 >= 1/astringent && current[i]/currentAverage1 <= astringent)
            {
                currentTotal += current[i];
                currentCount++;
            }
            else if(0 == current[i])
            {

            }
            else
            {
                TRACE_MARK("Give up data %u.\t-%u\n",current[i],astringent);
                current[i] = 0;
            }
        }
        int32_t currentAverage2 = currentTotal/currentCount;

        currentTotal = 0;
        currentCount = 0;
        astringent = 80;
        for(uint8_t i = 0;i<CHECK_TIMES;i++)
        {
            if(current[i]/currentAverage2 >= 1/astringent && current[i]/currentAverage2 <= astringent)
            {
                currentTotal += current[i];
                currentCount++;
            }
            else if(0 == current[i])
            {

            }
            else
            {
                TRACE_MARK("Give up data %u.\t-%u\n",current[i],astringent);
                current[i] = 0;
            }
        }
        int32_t currentAverage3 = currentTotal/currentCount;

        currentTotal = 0;
        currentCount = 0;
        astringent = 60;
        for(uint8_t i = 0;i<CHECK_TIMES;i++)
        {
            if(current[i]/currentAverage3 >= 1/astringent && current[i]/currentAverage3 <= astringent)
            {
                currentTotal += current[i];
                currentCount++;
            }
            else if(0 == current[i])
            {

            }
            else
            {
                TRACE_MARK("Give up data %u.\t-%u\n",current[i],astringent);
                current[i] = 0;
            }
        }
        int32_t currentAverage4 = currentTotal/currentCount;

        currentTotal = 0;
        currentCount = 0;
        astringent = 40;
        for(uint8_t i = 0;i<CHECK_TIMES;i++)
        {
            if(current[i]/currentAverage4 >= 1/astringent && current[i]/currentAverage4 <= astringent)
            {
                currentTotal += current[i];
                currentCount++;
            }
            else if(0 == current[i])
            {

            }
            else
            {
                TRACE_MARK("Give up data %u.\t-%u\n",current[i],astringent);
                current[i] = 0;
            }
        }
        volatile int32_t currentAverage5 = currentTotal/currentCount;
        if(currentAverage5<0) currentAverage5=0;
        volatile int32_t avgCurrent = avgCurrentCount(currentAverage5, true, 8);
        if(avgCurrent<0) avgCurrent=0;
        volatile int32_t thisCurrent = avgCurrentCount(currentAverage5, false, 1);
        if(thisCurrent<0) thisCurrent=0;

        TRACE_MARK("50�βɼ���20msÿ�Σ�ƽ����ѹΪ��%u uV\t-%u uV\t-%u uV\n", currentAverage1,currentAverage3,currentAverage5);
        TRACE_MARK("%d�ε���ͳ�ƺ�Ľ��Ϊ��>>>%u \n", AVG_TIMES_COUNT, avgCurrent);
        Hal_oled_currentChange(avgCurrent, thisCurrent);
    }
}
