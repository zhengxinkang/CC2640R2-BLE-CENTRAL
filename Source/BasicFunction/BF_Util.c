/**************************************************************************************************
  Filename:       BF_Util.c
  Revised:        $Date: 2017-07-18 18:18:13 $
  Revision:       $Revision: V0.1 $
  Programmer£º    YangGang
  Description:    This file contains the common application.
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "BF_Util.h"
#include "gapgattserver.h"
#include "hci_ext.h"

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */
typedef struct bf_timerStruct_t
{
  bfUtilCB_t    timer_callback;
  Clock_Struct  *timer_callbackClock;
  bool          isTimerHandle;
}bf_timerStruct_t;

/*********************************************************************
 * MACROS
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
// Queue object used for app messages
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;
static ICall_Semaphore mySemaphore;

static uint8_t index = 0;
static uint16_t thisEvent = 0;
static bf_timerStruct_t bf_timerArray[BF_UTIL_HANDLER_MAX_NUM];

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void BF_TimerTimeoutHandler(UArg a0)
{
  BF_UtilQueue_enqueueMsg(thisEvent,a0,NULL);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      BF_UtilQueue_Init
 *
 * @brief   Initialize a message Queue.
 *
 * @param   semaphore - Semaphore used to post events to the application thread.
 *
 * @return  None.
 */
void BF_UtilQueue_Init(ICall_Semaphore semaphore)
{
  // Create an RTOS queue for message from profile to be sent to app.
  appMsgQueue = Util_constructQueue(&appMsg);
  mySemaphore = semaphore;
}

/*********************************************************************
 * @fn      BF_UtilQueue_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event - message event.
 * @param   state - message state.
 *
 * @return  None.
 */
uint8_t BF_UtilQueue_enqueueMsg(uint16_t event, uint8_t state, uint8_t *pData)
{
    sbpEvt_t *pMsg = ICall_malloc(sizeof(sbpEvt_t));
    // Create dynamic pointer to message.
    if (pMsg)
    {
        pMsg->hdr.event = event;
        pMsg->hdr.state = state;
        pMsg->pData = pData;

        // Enqueue the message.
        return Util_enqueueMsg(appMsgQueue, mySemaphore, (uint8_t *) pMsg);
    }

    return FALSE;
}

//static uint8_t SimpleBLECentral_enqueueMsg(uint8_t event, uint8_t state,
//                                           uint8_t *pData)
//{
//  sbcEvt_t *pMsg = ICall_malloc(sizeof(sbcEvt_t));
//
//  // Create dynamic pointer to message.
//  if (pMsg)
//  {
//    pMsg->hdr.event = event;
//    pMsg->hdr.state = state;
//    pMsg->pData = pData;
//
//    // Enqueue the message.
//    return Util_enqueueMsg(appMsgQueue, syncEvent, (uint8_t *)pMsg);
//  }
//
//  return FALSE;
//}


bool BF_UtilQueueEmpty(void)
{
  return Queue_empty(appMsgQueue);
}

sbpEvt_t* BF_UtilDeQueue(void)
{
  return (sbpEvt_t*)Util_dequeueMsg(appMsgQueue);
}



void BF_UtilEvtCallbackInit(uint16_t event)
{
  thisEvent = event;
}

bool BF_UtilRegeistTimerCallback(bfUtilCB_t hanlder, uint8_t *thisIndex, uint32_t clockDuration)
{
  if(index < BF_UTIL_HANDLER_MAX_NUM)
  {
    bf_timerArray[index].timer_callbackClock = (Clock_Struct*)ICall_malloc(sizeof(Clock_Struct));
    bf_timerArray[index].timer_callback = hanlder;
    bf_timerArray[index].isTimerHandle = true;
    Util_constructClock(bf_timerArray[index].timer_callbackClock, BF_TimerTimeoutHandler, clockDuration, 0, false, index);
    *thisIndex = index;
    index++;
    return true;
  }
  return false;
}

bool BF_UtilRegeistItrCallback(bfUtilCB_t hanlder,uint8_t *hanlderIndex)
{
  if(index < BF_UTIL_HANDLER_MAX_NUM)
  {
    bf_timerArray[index].timer_callback = hanlder;
    bf_timerArray[index].isTimerHandle = false;
    *hanlderIndex = index;
    index++;
    return true;
  }
  return false;
}

void BF_UtilWaitHandle(uint8_t thisIndex)
{
  if(thisIndex < index)
  {
    if(bf_timerArray[thisIndex].timer_callback != NULL)
    {
      if(bf_timerArray[thisIndex].isTimerHandle)
      {
        if(!Util_isActive(bf_timerArray[thisIndex].timer_callbackClock))
        {
          Util_startClock(bf_timerArray[thisIndex].timer_callbackClock);
        }
      }
      else
      {
        BF_UtilQueue_enqueueMsg(thisEvent,thisIndex,NULL);
      }
    }
  }
}

/*********************************************************************
 * @fn      BF_UtilProcessEvt
 *
 * @brief   Process a pending Timer event.
 *
 * @param   timerID - timer ID.
 *
 * @return  None.
 */
void BF_UtilProcessEvt(uint8_t timerID)
{
  if(timerID < index)
  {
    if(bf_timerArray[timerID].timer_callback != NULL)
    {
      (*(bf_timerArray[timerID].timer_callback))();
    }
  }
}


//void BF_UtilReverseArray(uint8_t *array, uint8_t len)
//{
//  uint8_t temp,num = len / 2;
//  for(uint8_t i = 0; i < num; i++)
//  {
//    temp = *(array + i);
//    *(array + i) = *(array + len - i - 1);
//    *(array + len - i - 1) = temp;
//  }
//}

//bool BF_UtilCompairHex(uint8_t *hexA, uint8_t *hexB, uint8_t len)
//{
//  uint8_t i;
//  for(i = 0;i < len;i++)
//  {
//    if(*(hexA + i) != *(hexB + i))
//    {
//      return false;
//    }
//  }
//  return true;
//}


//void BF_UtilGetRand(void *array, uint8_t len, uint8_t size)
//{
//  uint8_t i;
//  if(size == sizeof(uint8_t))
//  {
//    for(i = 0; i < len; i++)
//    {
//      *((uint8_t*)array + i) = (Util_GetTRNG() & 0xff);
//    }
//  }
//  else if(size == sizeof(uint16_t))
//  {
//    for(i = 0; i < len; i++)
//    {
//      *((uint16_t*)array + i) = (Util_GetTRNG() & 0xffff);
//    }
//  }
//  else if(size == sizeof(uint32_t))
//  {
//    for(i = 0; i < len; i++)
//    {
//      *((uint32_t*)array + i) = Util_GetTRNG();
//    }
//  }
//}

void BF_UtilConverByteBigEndian(uint8_t *array, void *data, uint8_t size, bool toByte)
{
  uint8_t i;
  if(size == sizeof(uint16_t))
  {
    for(i = 0; i < 2; i++)
    {
      if(toByte)
      {
        *(array + i) = (*(uint16_t*)data >> ((1 - i) * 8)) & 0xff;
      }
      else
      {
        *(uint16_t*)data <<= 8;
        *(uint16_t*)data += *(array + i);
      }
    }
  }
  else if(size == sizeof(uint32_t))
  {
    for(i = 0; i < 4; i++)
    {
      if(toByte)
      {
        *(array + i) = (*(uint32_t*)data >> ((3 - i) * 8)) & 0xff;
      }
      else
      {
        *(uint32_t*)data <<= 8;
        *(uint32_t*)data += *(array + i);
      }
    }
  }
}


