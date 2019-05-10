/**************************************************************************************************
  Filename:       internal_flash.c
  Revised:        $Date: 2017-07-17 23:51:35 $
  Revision:       $Revision: V0.1 $
  Programmer¡êo    YangGang
  Description:    This file contains the internal flash function.
**************************************************************************************************/
#include "hal_types.h"
#include <driverlib/vims.h>
#include "hal_board_cfg.h"
#include "Driver_internalFlash.h"
#include "string.h"
#include "osal_snv.h"
#include "bcomdef.h"

flashParam_ts flashParam;

static void app_internal_flash_enable_cache(uint8_t state)
{
  if ( state != VIMS_MODE_DISABLED )
  {
    // Enable the Cache.
    VIMSModeSet( VIMS_BASE, VIMS_MODE_ENABLED );
  }
}

static uint8_t app_internal_flash_disable_cache(void)
{
  uint8_t state = VIMSModeGet( VIMS_BASE );

  // Check VIMS state
  if ( state != VIMS_MODE_DISABLED )
  {
    // Invalidate cache
    VIMSModeSet( VIMS_BASE, VIMS_MODE_DISABLED );

    // Wait for disabling to be complete
    while ( VIMSModeGet( VIMS_BASE ) != VIMS_MODE_DISABLED );

  }

  return state;
}

void flashParamReadBlock(uint32_t flash_addr, uint16_t len, uint8_t *buf)
{
  halIntState_t cs;
  uint8_t *ptr = (uint8_t *)(flash_addr);
  // Enter critical section.
  HAL_ENTER_CRITICAL_SECTION(cs);

  // Read from pointer into buffer.
  while (len--)
  {
    *buf++ = *ptr++;
  }

  // Exit critical section.
  HAL_EXIT_CRITICAL_SECTION(cs);
}

void flashParamWriteBlock(uint32_t flash_addr, uint16_t len, uint8_t *buf)
{
  uint8_t cacheState;

  cacheState = app_internal_flash_disable_cache();

  FlashProgram(buf, flash_addr, len);

  app_internal_flash_enable_cache(cacheState);
}

void flashParamEraseBlock(uint32_t addr)
{
  uint8_t cacheState;

  cacheState = app_internal_flash_disable_cache();

  FlashSectorErase(addr);

  app_internal_flash_enable_cache(cacheState);
}

bool flashParamParamEmpty(void)
{
  uint8_t cnt0 = 0, cntf = 0;
  flashParamParamRead();
  for(uint8_t i = 0; i < sizeof(flashParam_ts); i++)
  {
    if(((uint8_t*)&flashParam)[i] == 0x00)
    {
      cnt0++;
    }
    if(((uint8_t*)&flashParam)[i] == 0xff)
    {
      cntf++;
    }
    ((uint8_t*)&flashParam)[i] = 0xff;
  }
  if(cnt0 == sizeof(flashParam_ts))
  {
    osal_snv_write(BLE_NVID_CUST_START, sizeof(flashParam_ts), &flashParam);
    return true;
  }
  if(cntf == sizeof(flashParam_ts))
  {
    return true;
  }
  flashParamParamRead();
  return false;
}

void flashParamParamRead(void)
{
  osal_snv_read(BLE_NVID_CUST_START, sizeof(flashParam_ts), &flashParam);
}

void flashParamParamWrite(void)
{
  uint8_t s = osal_snv_write(BLE_NVID_CUST_START, sizeof(flashParam_ts), &flashParam);
}
