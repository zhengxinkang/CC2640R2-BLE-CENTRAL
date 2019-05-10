/**************************************************************************************************
  Filename:       internal_flash.h
  Revised:        $Date: 2017-07-17 23:51:35 $
  Revision:       $Revision: V5.16 $
  Programmer¡êo    YangGang
  Description:    This file contains the internal flash function.
**************************************************************************************************/

#ifndef SOURCE_DRIVERS_DRIVER_INTERNALFLASH_H_
#define SOURCE_DRIVERS_DRIVER_INTERNALFLASH_H_


#include "stdint.h"
#include "BF_Util.h"
//#include "osal_clock.h"
#include "UTC_clock.h"

/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * TYPEDEFS
 */
typedef struct
{
  uint32_t time;
  uint32_t currentOffset;
  uint32_t abnormalIndex;
}flashParam_ts;

extern flashParam_ts flashParam;

/*********************************************************************
 * API FUNCTIONS
 */
extern void flashParamReadBlock(uint32_t flash_addr, uint16_t len, uint8_t *buf);
extern void flashParamWriteBlock(uint32_t flash_addr, uint16_t len, uint8_t *buf);
extern void flashParamEraseBlock(uint32_t addr);

extern bool flashParamParamEmpty(void);
extern void flashParamParamRead(void);
extern void flashParamParamWrite(void);



#endif /* SOURCE_DRIVERS_DRIVER_INTERNALFLASH_H_ */
