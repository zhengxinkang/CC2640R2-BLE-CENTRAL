#ifndef __DRIVER_SPI__
#define __DRIVER_SPI__

#include <stdlib.h>
#include <stdbool.h>
#include "BF_Util.h"

#define EXT_FLASH_PAGE_SIZE   4096

bool extFlash_open(void);
void extFlash_close(void);
int  extFlash_readId(void);
bool extFlash_read(size_t offset, size_t length, uint8_t *buf);
bool extFlash_write(size_t offset, size_t length, const uint8_t *buf);
bool extFlash_eraseChip();
bool extFlash_erase(size_t offset, size_t length);
void extFlash_close(void);
bool extFlashPowerStandby(void);
int ExtFlash_powerDown(void);
bool extFlashPowerDown(void);
#endif
