#include "Driver_spi.h"
#include <ti/drivers/PIN.h>
#include "Board.h"
//#include "ExtFlash.h"
//#include "string.h"
#include <ti/drivers/spi/SPICC26XXDMA.h>
#include <ti/drivers/dma/UDMACC26XX.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/ssi.h)

/* Instruction codes */
#define BLS_CODE_PROGRAM          0x02 /**< Page Program */
#define BLS_CODE_READ             0x03 /**< Read Data */
#define BLS_CODE_READ_STATUS      0x05 /**< Read Status Register */
#define BLS_CODE_WRITE_ENABLE     0x06 /**< Write Enable */
#define BLS_CODE_SECTOR_ERASE     0x20 /**< Sector Erase */
#define BLS_CODE_MDID             0x90 /**< Manufacturer Device ID */

#define BLS_CODE_DP               0xB9 /**< Power down */
#define BLS_CODE_RDP              0xAB /**< Power standby */

/* Erase instructions */
#define BLS_CODE_ERASE_4K         0x20 /**< Sector Erase */
#define BLS_CODE_ERASE_32K        0x52
#define BLS_CODE_ERASE_64K        0xD8
#define BLS_CODE_ERASE_ALL        0xC7 /**< Mass Erase */

/* Bitmasks of the status register */
#define BLS_STATUS_SRWD_BM        0x80
#define BLS_STATUS_BP_BM          0x0C
#define BLS_STATUS_WEL_BM         0x02
#define BLS_STATUS_WIP_BM         0x01

#define BLS_STATUS_BIT_BUSY       0x01 /**< Busy bit of the status register */

/* Part specific constants */
#define BLS_PROGRAM_PAGE_SIZE     256
#define BLS_ERASE_SECTOR_SIZE     4096

/* Manufacturer IDs */
#define MF_MACRONIX               0xC2
#define MF_WINBOND                0xEF

/* -----------------------------------------------------------------------------
*  Local variables
* ------------------------------------------------------------------------------
*/
static PIN_Handle hFlashPin = NULL;
static PIN_State pinState;

// SPI interface
static SPI_Handle spiHandle = NULL;
static SPI_Params spiParams;

static PIN_Config BoardFlashPinTable[] =
{
    Board_SPI_FLASH_CS | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN, /* Ext. flash chip select */

    PIN_TERMINATE
};

/*******************************************************************************
*
*   SPI interface
*
*******************************************************************************/
static int Spi_write(const uint8_t *buf, size_t len);
static void Spi_flash(void);
static int Spi_read(uint8_t *buf, size_t len);
/*******************************************************************************
 * @fn          extFlashSelect
 *
 * @brief       Select the external flash on the SensorTag
 *
 * @param       none
 *
 * @return      none
 */
static void extFlashSelect(void)
{
    PIN_setOutputValue(hFlashPin,Board_SPI_FLASH_CS,Board_FLASH_CS_ON);
}

/*******************************************************************************
* @fn          extFlashDeselect
*
* @brief       Deselect the external flash on the SensorTag
*
* @param       none
*
* @return      none
*/
static void extFlashDeselect(void)
{
    PIN_setOutputValue(hFlashPin,Board_SPI_FLASH_CS,Board_FLASH_CS_OFF);
}

/**
* Wait till previous erase/program operation completes.
* @return Zero when successful.
*/
static int extFlash_waitReady(void)
{
    const uint8_t wbuf[1] = { BLS_CODE_READ_STATUS };
    int ret;

    /* Throw away all garbage */
    extFlashSelect();
    Spi_flash();
    extFlashDeselect();

    for (;;)
    {
        uint8_t buf;

        extFlashSelect();
        Spi_write(wbuf, sizeof(wbuf));
        ret = Spi_read(&buf,sizeof(buf));
        extFlashDeselect();

        if (ret)
        {
            /* Error */
            return -2;
        }
        if (!(buf & BLS_STATUS_BIT_BUSY))
        {
            /* Now ready */
            break;
        }
    }

    return 0;
}

/**
* Enable write.
* @return Zero when successful.
*/
static int extFlash_writeEnable(void)
{
    const uint8_t wbuf[] = { BLS_CODE_WRITE_ENABLE };

    extFlashSelect();
    int ret = Spi_write(wbuf,sizeof(wbuf));
    extFlashDeselect();

    if (ret)
    {
        return -3;
    }
    return 0;
}

/*******************************************************************************
* @fn          Spi_write
*
* @brief       Write to an SPI device
*
* @param       buf - pointer to data buffer
* @param       len - number of bytes to write
*
* @return      '0' if success, -1 if failed
*/
static int Spi_write(const uint8_t *buf, size_t len)
{
    SPI_Transaction masterTransaction;

    masterTransaction.count  = len;
    masterTransaction.txBuf  = (void*)buf;
    masterTransaction.arg    = NULL;
    masterTransaction.rxBuf  = NULL;

    return SPI_transfer(spiHandle, &masterTransaction) ? 0 : -1;
}


/*******************************************************************************
* @fn          Spi_read
*
* @brief       Read from an SPI device
*
* @param       buf - pointer to data buffer
* @param       len - number of bytes to write
*
* @return      '0' if success, -1 if failed
*/
static int Spi_read(uint8_t *buf, size_t len)
{
    SPI_Transaction masterTransaction;

    masterTransaction.count = len;
    masterTransaction.txBuf = NULL;
    masterTransaction.arg = NULL;
    masterTransaction.rxBuf = buf;

    return SPI_transfer(spiHandle, &masterTransaction) ? 0 : -1;
}


/*******************************************************************************
* @fn          Spi_open
*
* @brief       Open the RTOS SPI driver
*
* @param       bitRate - transfer speed in bits/sec
*
* @return      true if success
*/
static bool Spi_open(uint32_t bitRate)
{
    /*  Configure SPI as master */
    SPI_Params_init(&spiParams);
    spiParams.bitRate = bitRate;
    spiParams.mode = SPI_MASTER;
    spiParams.transferMode = SPI_MODE_BLOCKING;

    /* Attempt to open SPI. */
    spiHandle = SPI_open(Board_SPI0, &spiParams);

    return spiHandle != NULL;
}

/*******************************************************************************
* @fn          Spi_close
*
* @brief       Close the RTOS SPI driver
*
* @return      none
*/
static void Spi_close(void)
{
    if (spiHandle != NULL)
    {
        // Close the RTOS driver
        SPI_close(spiHandle);
        spiHandle = NULL;
    }
}

/*******************************************************************************
* @fn          Spi_flash
*
* @brief       Get rid of garbage from the slave
*
* @param       none
*
* @return      none
*/
static void Spi_flash(void)
{
    /* make sure SPI hardware module is done  */
    while(SSIBusy(((SPICC26XXDMA_HWAttrsV1*)spiHandle->hwAttrs)->baseAddr))
    { };
}



/* See ExtFlash.h file for description */
bool extFlash_open(void)
{
    bool f;

    hFlashPin = PIN_open(&pinState, BoardFlashPinTable);

    if (hFlashPin == NULL)
    {
        return false;
    }

    /* Initialise SPI. Subsequent calls will do nothing. */
    SPI_init();

    /* Make sure SPI is available */
    f = Spi_open(1000000);

    if (f)
    {
        /* Put the part is standby mode */
//        f = extFlashPowerStandby();
//        f = extFlashPowerDown();
//
//        if (!f)
//        {
//            extFlash_close();
//        }
    }

    return f;
}

void extFlash_close(void)
{
    if (hFlashPin != NULL)
    {
        // Put the part in low power mode
        extFlashPowerDown();

        /* Make sure SPI lines have a defined state */
        Spi_close();

        PIN_close(hFlashPin);
        hFlashPin = NULL;
    }
}

int  extFlash_readId(void)
{
    uint8_t wbuf[4];

    /* Wait till previous erase/program operation completes */
    int ret = extFlash_waitReady();
    if (ret)
    {
        return false;
    }

    /* SPI is driven with very low frequency (1MHz < 33MHz fR spec)
    * in this temporary implementation.
    * and hence it is not necessary to use fast read. */
    wbuf[0] = BLS_CODE_MDID;
    wbuf[1] = 0x00;
    wbuf[2] = 0x00;
    wbuf[3] = 0x00;

    extFlashSelect();

    if (Spi_write(wbuf, sizeof(wbuf)))
    {
        /* failure */
        extFlashDeselect();
        return false;
    }
    uint8_t buf[2];
    ret = Spi_read(buf, 2);
    int32_t id= (buf[0]<<8)+buf[1];

    extFlashDeselect();

    return id;
}

/* See ExtFlash.h file for description */
bool extFlash_read(size_t offset, size_t length, uint8_t *buf)
{
    uint8_t wbuf[4];

    /* Wait till previous erase/program operation completes */
    int ret = extFlash_waitReady();
    if (ret)
    {
        return false;
    }

    /* SPI is driven with very low frequency (1MHz < 33MHz fR spec)
    * in this temporary implementation.
    * and hence it is not necessary to use fast read. */
    wbuf[0] = BLS_CODE_READ;
    wbuf[1] = (offset >> 16) & 0xff;
    wbuf[2] = (offset >> 8) & 0xff;
    wbuf[3] = offset & 0xff;

    extFlashSelect();

    if (Spi_write(wbuf, sizeof(wbuf)))
    {
        /* failure */
        extFlashDeselect();
        return false;
    }

    ret = Spi_read(buf, length);

    extFlashDeselect();

    return ret == 0;
}
/* See ExtFlash.h file for description */
bool extFlash_write(size_t offset, size_t length, const uint8_t *buf)
{
    uint8_t wbuf[4];

    while (length > 0)
    {
        /* Wait till previous erase/program operation completes */
        int ret = extFlash_waitReady();
        if (ret)
        {
            return false;
        }

        ret = extFlash_writeEnable();
        if (ret)
        {
            return false;
        }

        size_t ilen; /* interim length per instruction */

        ilen = BLS_PROGRAM_PAGE_SIZE - (offset % BLS_PROGRAM_PAGE_SIZE);
        if (length < ilen)
        {
            ilen = length;
        }

        wbuf[0] = BLS_CODE_PROGRAM;
        wbuf[1] = (offset >> 16) & 0xff;
        wbuf[2] = (offset >> 8) & 0xff;
        wbuf[3] = offset & 0xff;

        offset += ilen;
        length -= ilen;

        /* Up to 100ns CS hold time (which is not clear
        * whether it's application only in between reads)
        * is not imposed here since above instructions
        * should be enough to delay
        * as much. */
        extFlashSelect();

        if (Spi_write(wbuf, sizeof(wbuf)))
        {
            /* failure */
            extFlashDeselect();
            return false;
        }

        if (Spi_write(buf,ilen))
        {
            /* failure */
            extFlashDeselect();
            return false;
        }
        buf += ilen;
        extFlashDeselect();
    }

    return true;
}

bool extFlash_eraseChip()
{
    uint8_t cmd;
    bool success;


    int ret = extFlash_waitReady();
    if (ret)
    {
        return false;
    }
    ret = extFlash_writeEnable();
    if (ret)
    {
        return false;
    }


    cmd = BLS_CODE_ERASE_ALL;
    extFlashSelect();
    success = Spi_write(&cmd,sizeof(cmd)) == 0;
    extFlashDeselect();

    if (success)
    {
        if (extFlash_waitReady() != 0)
        {
            success = false;
        }
    }

    return success;
}

/* See ExtFlash.h file for description */
bool extFlash_erase(size_t offset, size_t length)
{
    /* Note that Block erase might be more efficient when the floor map
    * is well planned for OTA but to simplify for the temporary implementation,
    * sector erase is used blindly. */
    uint8_t wbuf[4];
    size_t i, numsectors;

    wbuf[0] = BLS_CODE_SECTOR_ERASE;

    {
        size_t endoffset = offset + length - 1;
        offset = (offset / BLS_ERASE_SECTOR_SIZE) * BLS_ERASE_SECTOR_SIZE;
        numsectors = (endoffset - offset + BLS_ERASE_SECTOR_SIZE - 1) /
            BLS_ERASE_SECTOR_SIZE;
    }

    for (i = 0; i < numsectors; i++)
    {
        /* Wait till previous erase/program operation completes */
        int ret = extFlash_waitReady();
        if (ret)
        {
            return false;
        }

        ret = extFlash_writeEnable();
        if (ret)
        {
            return false;
        }

        wbuf[1] = (offset >> 16) & 0xff;
        wbuf[2] = (offset >> 8) & 0xff;
        wbuf[3] = offset & 0xff;

        extFlashSelect();

        if (Spi_write(wbuf, sizeof(wbuf)))
        {
            /* failure */
            extFlashDeselect();
            return false;
        }
        extFlashDeselect();

        offset += BLS_ERASE_SECTOR_SIZE;
    }

    return true;
}


/*******************************************************************************
* @fn       extFlashPowerStandby
*
* @brief    Take device out of power save mode and prepare it for normal operation
*
* @param    none
*
* @return   Returns true if command successfully written
*******************************************************************************/
bool extFlashPowerStandby(void)
{
    uint8_t cmd;
    bool success;

    cmd = BLS_CODE_RDP;
    extFlashSelect();
    success = Spi_write(&cmd,sizeof(cmd)) == 0;
    extFlashDeselect();

    if (success)
    {
        if (extFlash_waitReady() != 0)
        {
            success = false;
        }
    }

    return success;
}
/*******************************************************************************
* @fn       extFlashPowerDown
*
* @brief    Put the device in power save mode. No access to data; only
*           the status register is accessible.
*
* @param    none
*
* @return   Returns true if transactions succeed
*******************************************************************************/
bool extFlashPowerDown(void)
{
    uint8_t cmd;
    bool success;

    cmd = BLS_CODE_DP;
    extFlashSelect();
    success = Spi_write(&cmd,sizeof(cmd)) == 0;
    extFlashDeselect();

    return success;
}
