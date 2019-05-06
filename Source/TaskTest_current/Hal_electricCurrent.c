/*
 * Hal_electricCurrent.c
 *
 *  Created on: 2018年11月16日
 *      Author: zheng
 */
#include "BF_Util.h"
#include "Driver_ADCmpc3901.h"
#include "Trace.h"
#include "Hal_electricCurrent.h"
#include "Hal_oled.h"

#define TIMER_CURRENT_PERIOD 10
static uint8_t currentTimeIndex;

static void current_timeoutHandler();

void Hal_electricCurrent_init()
{
    BF_UtilRegeistTimerCallback(current_timeoutHandler, &currentTimeIndex, TIMER_CURRENT_PERIOD);
    BF_UtilWaitHandle(currentTimeIndex);
}

#define AVG_TIMES_COUNT         8
static uint32_t currentAvg[AVG_TIMES_COUNT]={0,0,0,0,0,0,0,0};
static bool isCurrentAvgActive = false;

#define CHECK_TIMES 50
static uint32_t current[CHECK_TIMES];
static uint16_t currentIndex = 0;

/*
currentThis:  要更新的本次电流
isShift:      本次是否需要更新
times:        需要统计几次电流值
 */
uint32_t avgCurrentCount(uint32_t currentThis, bool isShift, uint8_t times)
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

    //计算平均值
    uint32_t total = 0;
    for(uint8_t i=0; i<times; i++)
    {
        total += currentAvg[i];
    }
    return total/times - 10;
}

static void current_timeoutHandler()
{
    BF_UtilWaitHandle(currentTimeIndex);
    current[currentIndex++]  = Read_1CH_adc(0);

    if(currentIndex == CHECK_TIMES)
    {
        currentIndex = 0;
        uint32_t currentTotal = 0;
        uint8_t currentCount = 0;
        for(uint8_t i = 0;i<CHECK_TIMES;i++)
        {
            TRACE_MARK("第%02d电流大小%d uA。\n",i, current[i]);

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
        uint32_t currentAverage1 = currentTotal/currentCount;

        uint32_t avgCurrent = avgCurrentCount(currentAverage1, true, 8);
        uint32_t thisCurrent = avgCurrentCount(currentAverage1, false, 1);
//        TRACE_DEBUG("50次采集（20ms每次）平均电压为：%u uV\t-%u uV\t-%u uV\n", currentAverage1,currentAverage3,currentAverage5);
        TRACE_DEBUG("%d次电流统计后的结果为：>>>%u \n", AVG_TIMES_COUNT, avgCurrent);
        Hal_oled_currentChange( avgCurrent, thisCurrent);
    }
}
