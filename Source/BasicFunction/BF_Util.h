/**************************************************************************************************
**************************************************************************************************/
#ifndef __BF_UTIL_H__
#define __BF_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * INCLUDES
 */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>

#include "gatt.h"
#include "gattservapp.h"

#include "gapbondmgr.h"

#include "osal_snv.h"
#include "util.h"
//#include "ICallBleAPIMSG.h"
#include "stdint.h"
#include "stdbool.h"

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
#define BF_UTIL_HANDLER_MAX_NUM         25

/*********************************************************************
 * TYPEDEFS
 */
// App event passed from profiles.
typedef struct
{
  appEvtHdr_t hdr;  // event header.
  uint8_t *pData;  // event data
} sbpEvt_t;

#define sbcEvt_t sbpEvt_t

typedef void (*bfUtilCB_t)(void);

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * API FUNCTIONS
 */
/*********************************************************************
 * @fn      BF_UtilQueue_enqueueMsg
 *
 * @brief   Initialize a message Queue.
 *
 * @param   semaphore - Semaphore used to post events to the application thread.
 *
 * @return  None.
 */
void BF_UtilQueue_Init(ICall_Semaphore semaphore);

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
uint8_t BF_UtilQueue_enqueueMsg(uint16_t event, uint8_t state, uint8_t *pData);
bool BF_UtilQueueEmpty(void);
sbcEvt_t* BF_UtilDeQueue(void);

void BF_UtilEvtCallbackInit(uint16_t event);
bool BF_UtilRegeistTimerCallback(bfUtilCB_t hanlder, uint8_t *index, uint32_t clockDuration);
bool BF_UtilRegeistItrCallback(bfUtilCB_t hanlder,uint8_t *hanlderIndex);
void BF_UtilWaitHandle(uint8_t thisIndex);
void BF_UtilProcessEvt(uint8_t timerID);

//void BF_UtilReverseArray(uint8_t *array, uint8_t len);
bool BF_UtilCompairHex(uint8_t *hexA, uint8_t *hexB, uint8_t len);
//void BF_UtilGetRand(void *array, uint8_t len, uint8_t size);
void BF_UtilConverByteBigEndian(uint8_t *array, void *data, uint8_t size, bool toByte);
void BF_taskSleepMs(uint32_t ms);
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif  //__BF_UTIL_H__
