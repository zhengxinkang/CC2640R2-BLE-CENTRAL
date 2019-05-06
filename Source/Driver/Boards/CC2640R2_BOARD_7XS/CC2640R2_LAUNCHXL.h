/*
 * Copyright (c) 2016-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       CC2640R2_LAUNCHXL.h
 *
 *  @brief      CC2640R2 LaunchPad Board Specific header file.
 *
 *  This file is responsible for setting up the board specific items for the
 *  CC2640R2_LAUNCHXL board.
 *
 *  This board file is made for the 7x7 mm QFN package, to convert this board
 *  file to use for other smaller device packages please refer to the table
 *  below which lists the max IOID values supported by each package. All other
 *  unused pins should be set to IOID_UNUSED.
 *
 *  Furthermore the board file is also used
 *  to define a symbol that configures the RF front end and bias.
 *  See the comments below for more information.
 *  For an in depth tutorial on how to create a custom board file, please refer
 *  to the section "Running the SDK on Custom Boards" with in the Software
 *  Developer's Guide.
 *
 *  Refer to the datasheet for all the package options and IO descriptions:
 *  http://www.ti.com/lit/ds/symlink/cc2640r2f.pdf
 *
 *  +-----------------------+------------------+-----------------------+
 *  |     Package Option    |  Total GPIO Pins |   MAX IOID            |
 *  +=======================+==================+=======================+
 *  |     7x7 mm QFN        |     31           |   IOID_30             |
 *  +-----------------------+------------------+-----------------------+
 *  |     5x5 mm QFN        |     15           |   IOID_14             |
 *  +-----------------------+------------------+-----------------------+
 *  |     4x4 mm QFN        |     10           |   IOID_9              |
 *  +-----------------------+------------------+-----------------------+
 *  |     2.7 x 2.7 mm WCSP |     14           |   IOID_13             |
 *  +-----------------------+------------------+-----------------------+
 *  ============================================================================
 */
#ifndef __CC2640R2_LAUNCHXL_BOARD_H__
#define __CC2640R2_LAUNCHXL_BOARD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <ti/drivers/PIN.h>
#include <ti/devices/cc26x0r2/driverlib/ioc.h>

/* Externs */
extern const PIN_Config BoardGpioInitTable[];

/* Defines */
#ifndef CC2640R2_LAUNCHXL
  #define CC2640R2_LAUNCHXL
#endif /* CC2640R2_LAUNCHXL */

/*
 *  ============================================================================
 *  RF Front End and Bias configuration symbols for TI reference designs and
 *  kits. This symbol sets the RF Front End configuration in ble_user_config.h
 *  and selects the appropriate PA table in ble_user_config.c.
 *  Other configurations can be used by editing these files.
 *
 *  Define only one symbol:
 *  CC2650EM_7ID    - Differential RF and internal biasing
                      (default for CC2640R2 LaunchPad)
 *  CC2650EM_5XD    �C Differential RF and external biasing
 *  CC2650EM_4XS    �C Single-ended RF on RF-P and external biasing
 *  CC2640R2DK_CXS  - WCSP: Single-ended RF on RF-N and external biasing
 *                    (Note that the WCSP is only tested and characterized for
 *                     single ended configuration, and it has a WCSP-specific
 *                     PA table)
 *
 *  Note: CC2650EM_xxx reference designs apply to all CC26xx devices.
 *  ==========================================================================
 */
#define CC2650EM_7ID

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>                  <pin mapping>
 */

//---define start---------------------------------------------------------------------------------------
#define PORT_ADC_DR                             IOID_0
#define PORT_ADC_CS                             IOID_1
//#define PORT_ADC_SCK                            IOID_27
//#define PORT_ADC_SDO                            IOID_28
//#define PORT_ADC_SDI                            IOID_29
//#define PORT_ADC_RST                            IOID_30

//uartEmulator����<scif.h>�ж���
//#define CC2640R2_LAUNCHXL_EMULATOR_UART_RX    IOID_2         /* RXD */
//#define CC2640R2_LAUNCHXL_EMULATOR_UART_TX    IOID_3         /* TXD */

//��Դ����
#define PORT_POWER_BAT                          PIN_UNASSIGNED  //IOID_4
#define PORT_POWER_USB                          PIN_UNASSIGNED  //IOID_5

//LED��
#define CC2640R2_LAUNCHXL_PIN_RLED              PIN_UNASSIGNED  //IOID_6
#define CC2640R2_LAUNCHXL_PIN_GLED              PIN_UNASSIGNED  //IOID_7

#define PORT_HC595_SRCLRn                       IOID_4
#define PORT_HC595_SRCLK                        IOID_5
#define PORT_HC595_RCLK                         IOID_6
#define PORT_HC595_SER                          IOID_7
//#define HC595_OE

//�������ź�-GPIO����
#define PORT_MOTOR_FORWARD                      PIN_UNASSIGNED  //IOID_8
#define PORT_MOTOR_REVERSAL                     PIN_UNASSIGNED  //IOID_9

//��������-GPIO���
#define PORT_ANTI_LOCK                          PIN_UNASSIGNED  //IOID_10

//���˿���-GPIO���
#define PORT_PREVENT_DISMANTLE                  IOID_11

//Discrete Inputs ��������
#define CC2640R2_LAUNCHXL_PIN_BTN1              IOID_13
#define CC2640R2_LAUNCHXL_PIN_BTN2              PIN_UNASSIGNED  //IOID_14

//�����ź�
#define PORT_TEST_START                         PIN_UNASSIGNED  //IOID_16

#define PORT_HC165_SL                           IOID_8
#define PORT_HC165_CLK                          IOID_9
#define PORT_HC165_QH                           IOID_10

#define PORT_TEST_MODEL_S31                     PIN_UNASSIGNED  //IOID_17
#define PORT_TEST_MODEL_S31X                    PIN_UNASSIGNED  //IOID_18

//����ź�
#define PORT_TEST_OBLIQUE_LOCK_TONGUE           PIN_UNASSIGNED  //IOID_19 //б��
#define PORT_TEST_SETTING_BUTTON                PIN_UNASSIGNED  //IOID_20 //���ü�
#define PORT_TEST_CLEAR_BUTTON                  PIN_UNASSIGNED  //IOID_21 //��ռ�

#define CC2640R2_LAUNCHXL_PWMPIN0               IOID_15
//ADC����
#define PORT_SP_OUT                             IOID_16


#define PORT_OLED_SCLK                          IOID_18
#define PORT_OLED_SDIN                          IOID_19
#define PORT_OLED_RST                           IOID_20
#define PORT_OLED_DC                            IOID_21
#define PORT_OLED_CS                            IOID_22

#define CC2640R2_LAUNCHXL_DIO23_ANALOG          IOID_23

//��MCU��λ
#define PORT_TEST_RESET_TARGET                  IOID_24

//uart0
#define CC2640R2_LAUNCHXL_UART_TX               IOID_26         /* TXD */
#define CC2640R2_LAUNCHXL_UART_RX               IOID_25         /* RXD */

//#define PORT_SPI_DATA                           IOID_27
//#define PORT_SPI_CLK                            IOID_28

#define PORT_ADC_SCK                            IOID_27
#define PORT_ADC_SDO                            IOID_28
#define PORT_ADC_SDI                            IOID_29
#define PORT_ADC_RST                            IOID_30

//---define end---------------------------------------------------------------------------------------
#define BOARD_PORT_UART_RX                      CC2640R2_LAUNCHXL_UART_RX
#define BOARD_PORT_UART_TX                      CC2640R2_LAUNCHXL_UART_TX

/* Analog Capable DIOs */
//#define CC2640R2_LAUNCHXL_DIO23_ANALOG          IOID_23
#define CC2640R2_LAUNCHXL_DIO24_ANALOG          PIN_UNASSIGNED  //IOID_24
#define CC2640R2_LAUNCHXL_DIO25_ANALOG          PIN_UNASSIGNED  //IOID_25
#define CC2640R2_LAUNCHXL_DIO26_ANALOG          PIN_UNASSIGNED  //IOID_26
#define CC2640R2_LAUNCHXL_DIO27_ANALOG          PIN_UNASSIGNED  //IOID_27
#define CC2640R2_LAUNCHXL_DIO28_ANALOG          PIN_UNASSIGNED  //IOID_28
#define CC2640R2_LAUNCHXL_DIO29_ANALOG          PIN_UNASSIGNED  //IOID_29
#define CC2640R2_LAUNCHXL_DIO30_ANALOG          PIN_UNASSIGNED  //IOID_30

/* Digital IOs */
#define CC2640R2_LAUNCHXL_DIO0                  PIN_UNASSIGNED  //IOID_0
#define CC2640R2_LAUNCHXL_DIO1_RFSW             PIN_UNASSIGNED  //IOID_1
#define CC2640R2_LAUNCHXL_DIO12                 PIN_UNASSIGNED  //IOID_12
#define CC2640R2_LAUNCHXL_DIO15                 PIN_UNASSIGNED  //IOID_15
#define CC2640R2_LAUNCHXL_DIO16_TDO             PIN_UNASSIGNED  //IOID_16
#define CC2640R2_LAUNCHXL_DIO17_TDI             PIN_UNASSIGNED  //IOID_17
#define CC2640R2_LAUNCHXL_DIO21                 PIN_UNASSIGNED  //IOID_21
#define CC2640R2_LAUNCHXL_DIO22                 PIN_UNASSIGNED  //IOID_22

/* GPIO */
#define CC2640R2_LAUNCHXL_GPIO_LED_ON           1
#define CC2640R2_LAUNCHXL_GPIO_LED_OFF          0

/* I2C */
#define CC2640R2_LAUNCHXL_I2C0_SCL0             PIN_UNASSIGNED  //IOID_4
#define CC2640R2_LAUNCHXL_I2C0_SDA0             PIN_UNASSIGNED  //IOID_5

/* LCD (430BOOST - Sharp96 Rev 1.1) */
#define CC2640R2_LAUNCHXL_LCD_CS                PIN_UNASSIGNED  //IOID_24 /* SPI chip select */
#define CC2640R2_LAUNCHXL_LCD_EXTCOMIN          PIN_UNASSIGNED  //IOID_12 /* External COM inversion */
#define CC2640R2_LAUNCHXL_LCD_ENABLE            PIN_UNASSIGNED  //IOID_22 /* LCD enable */
#define CC2640R2_LAUNCHXL_LCD_POWER             PIN_UNASSIGNED  //IOID_23 /* LCD power control */
#define CC2640R2_LAUNCHXL_LCD_CS_ON             1
#define CC2640R2_LAUNCHXL_LCD_CS_OFF            0

/* LEDs */
#define CC2640R2_LAUNCHXL_PIN_LED_ON            1
#define CC2640R2_LAUNCHXL_PIN_LED_OFF           0

/* PWM Outputs */
//#define CC2640R2_LAUNCHXL_PWMPIN0               CC2640R2_LAUNCHXL_PIN_RLED
#define CC2640R2_LAUNCHXL_PWMPIN1               CC2640R2_LAUNCHXL_PIN_GLED
#define CC2640R2_LAUNCHXL_PWMPIN2               PIN_UNASSIGNED
#define CC2640R2_LAUNCHXL_PWMPIN3               PIN_UNASSIGNED
#define CC2640R2_LAUNCHXL_PWMPIN4               PIN_UNASSIGNED
#define CC2640R2_LAUNCHXL_PWMPIN5               PIN_UNASSIGNED
#define CC2640R2_LAUNCHXL_PWMPIN6               PIN_UNASSIGNED
#define CC2640R2_LAUNCHXL_PWMPIN7               PIN_UNASSIGNED

/* SPI */
#define CC2640R2_LAUNCHXL_SPI_FLASH_CS          PIN_UNASSIGNED  //IOID_20
#define CC2640R2_LAUNCHXL_FLASH_CS_ON           0
#define CC2640R2_LAUNCHXL_FLASH_CS_OFF          1

/* SPI Board */
#define CC2640R2_LAUNCHXL_SPI0_MISO             PIN_UNASSIGNED  //IOID_8          /* RF1.20 */
#define CC2640R2_LAUNCHXL_SPI0_MOSI             PIN_UNASSIGNED  //IOID_9          /* RF1.18 */
#define CC2640R2_LAUNCHXL_SPI0_CLK              PIN_UNASSIGNED  //IOID_10         /* RF1.16 */
#define CC2640R2_LAUNCHXL_SPI0_CSN              PIN_UNASSIGNED
#define CC2640R2_LAUNCHXL_SPI1_MISO             PIN_UNASSIGNED
#define CC2640R2_LAUNCHXL_SPI1_MOSI             PIN_UNASSIGNED
#define CC2640R2_LAUNCHXL_SPI1_CLK              PIN_UNASSIGNED
#define CC2640R2_LAUNCHXL_SPI1_CSN              PIN_UNASSIGNED

/* UART Board */
#define CC2640R2_LAUNCHXL_UART_CTS              PIN_UNASSIGNED  //IOID_19         /* CTS */
#define CC2640R2_LAUNCHXL_UART_RTS              PIN_UNASSIGNED  //IOID_18         /* RTS */

/*!
 *  @brief  Initialize the general board specific settings
 *
 *  This function initializes the general board specific settings.
 */
void CC2640R2_LAUNCHXL_initGeneral(void);

/*!
 *  @brief  Turn off the external flash on LaunchPads
 *
 */
void CC2640R2_LAUNCHXL_shutDownExtFlash(void);

/*!
 *  @def    CC2640R2_LAUNCHXL_ADCBufName
 *  @brief  Enum of ADCs
 */
typedef enum CC2640R2_LAUNCHXL_ADCBufName {
    CC2640R2_LAUNCHXL_ADCBUF0 = 0,

    CC2640R2_LAUNCHXL_ADCBUFCOUNT
} CC2640R2_LAUNCHXL_ADCBufName;

/*!
 *  @def    CC2640R2_LAUNCHXL_ADCBuf0SourceName
 *  @brief  Enum of ADCBuf channels
 */
typedef enum CC2640R2_LAUNCHXL_ADCBuf0ChannelName {
    CC2640R2_LAUNCHXL_ADCBUF0CHANNEL0 = 0,
    CC2640R2_LAUNCHXL_ADCBUF0CHANNEL1,
    CC2640R2_LAUNCHXL_ADCBUF0CHANNEL2,
    CC2640R2_LAUNCHXL_ADCBUF0CHANNEL3,
    CC2640R2_LAUNCHXL_ADCBUF0CHANNEL4,
    CC2640R2_LAUNCHXL_ADCBUF0CHANNEL5,
    CC2640R2_LAUNCHXL_ADCBUF0CHANNEL6,
    CC2640R2_LAUNCHXL_ADCBUF0CHANNEL7,
    CC2640R2_LAUNCHXL_ADCBUF0CHANNELVDDS,
    CC2640R2_LAUNCHXL_ADCBUF0CHANNELDCOUPL,
    CC2640R2_LAUNCHXL_ADCBUF0CHANNELVSS,

    CC2640R2_LAUNCHXL_ADCBUF0CHANNELCOUNT
} CC2640R2_LAUNCHXL_ADCBuf0ChannelName;

/*!
 *  @def    CC2640R2_LAUNCHXL_ADCName
 *  @brief  Enum of ADCs
 */
typedef enum CC2640R2_LAUNCHXL_ADCName {
    CC2640R2_LAUNCHXL_ADC0 = 0,
    CC2640R2_LAUNCHXL_ADC1,
    CC2640R2_LAUNCHXL_ADC2,
    CC2640R2_LAUNCHXL_ADC3,
    CC2640R2_LAUNCHXL_ADC4,
    CC2640R2_LAUNCHXL_ADC5,
    CC2640R2_LAUNCHXL_ADC6,
    CC2640R2_LAUNCHXL_ADC7,
    CC2640R2_LAUNCHXL_ADCDCOUPL,
    CC2640R2_LAUNCHXL_ADCVSS,
    CC2640R2_LAUNCHXL_ADCVDDS,

    CC2640R2_LAUNCHXL_ADCCOUNT
} CC2640R2_LAUNCHXL_ADCName;

/*!
 *  @def    CC2640R2_LAUNCHXL_CryptoName
 *  @brief  Enum of Crypto names
 */
typedef enum CC2640R2_LAUNCHXL_CryptoName {
    CC2640R2_LAUNCHXL_CRYPTO0 = 0,

    CC2640R2_LAUNCHXL_CRYPTOCOUNT
} CC2640R2_LAUNCHXL_CryptoName;

/*!
 *  @def    CC2640R2_LAUNCHXL_GPIOName
 *  @brief  Enum of GPIO names
 */
typedef enum CC2640R2_LAUNCHXL_GPIOName {
    CC2640R2_LAUNCHXL_GPIO_S1 = 0,
    CC2640R2_LAUNCHXL_GPIO_S2,
    CC2640R2_LAUNCHXL_GPIO_LED_GREEN,
    CC2640R2_LAUNCHXL_GPIO_LED_RED,
    CC2640R2_LAUNCHXL_GPIO_SPI_FLASH_CS,

    CC2640R2_LAUNCHXL_GPIOCOUNT
} CC2640R2_LAUNCHXL_GPIOName;

/*!
 *  @def    CC2640R2_LAUNCHXL_GPTimerName
 *  @brief  Enum of GPTimer parts
 */
typedef enum CC2640R2_LAUNCHXL_GPTimerName {
    CC2640R2_LAUNCHXL_GPTIMER0A = 0,
    CC2640R2_LAUNCHXL_GPTIMER0B,
    CC2640R2_LAUNCHXL_GPTIMER1A,
    CC2640R2_LAUNCHXL_GPTIMER1B,
    CC2640R2_LAUNCHXL_GPTIMER2A,
    CC2640R2_LAUNCHXL_GPTIMER2B,
    CC2640R2_LAUNCHXL_GPTIMER3A,
    CC2640R2_LAUNCHXL_GPTIMER3B,

    CC2640R2_LAUNCHXL_GPTIMERPARTSCOUNT
} CC2640R2_LAUNCHXL_GPTimerName;

/*!
 *  @def    CC2640R2_LAUNCHXL_GPTimers
 *  @brief  Enum of GPTimers
 */
typedef enum CC2640R2_LAUNCHXL_GPTimers {
    CC2640R2_LAUNCHXL_GPTIMER0 = 0,
    CC2640R2_LAUNCHXL_GPTIMER1,
    CC2640R2_LAUNCHXL_GPTIMER2,
    CC2640R2_LAUNCHXL_GPTIMER3,

    CC2640R2_LAUNCHXL_GPTIMERCOUNT
} CC2640R2_LAUNCHXL_GPTimers;

/*!
 *  @def    CC2640R2_LAUNCHXL_I2CName
 *  @brief  Enum of I2C names
 */
typedef enum CC2640R2_LAUNCHXL_I2CName {
    CC2640R2_LAUNCHXL_I2C0 = 0,

    CC2640R2_LAUNCHXL_I2CCOUNT
} CC2640R2_LAUNCHXL_I2CName;

/*!
 *  @def    CC2640R2_LAUNCHXL_PWM
 *  @brief  Enum of PWM outputs
 */
typedef enum CC2640R2_LAUNCHXL_PWMName {
    CC2640R2_LAUNCHXL_PWM0 = 0,
    CC2640R2_LAUNCHXL_PWM1,
    CC2640R2_LAUNCHXL_PWM2,
    CC2640R2_LAUNCHXL_PWM3,
    CC2640R2_LAUNCHXL_PWM4,
    CC2640R2_LAUNCHXL_PWM5,
    CC2640R2_LAUNCHXL_PWM6,
    CC2640R2_LAUNCHXL_PWM7,

    CC2640R2_LAUNCHXL_PWMCOUNT
} CC2640R2_LAUNCHXL_PWMName;
/*!
 *  @def    CC2640R2_LAUNCHXL_SPIName
 *  @brief  Enum of SPI names
 */
typedef enum CC2640R2_LAUNCHXL_SPIName {
    CC2640R2_LAUNCHXL_SPI0 = 0,
    CC2640R2_LAUNCHXL_SPI1,

    CC2640R2_LAUNCHXL_SPICOUNT
} CC2640R2_LAUNCHXL_SPIName;

/*!
 *  @def    CC2640R2_LAUNCHXL_UARTName
 *  @brief  Enum of UARTs
 */
typedef enum CC2640R2_LAUNCHXL_UARTName {
    CC2640R2_LAUNCHXL_UART0 = 0,

    CC2640R2_LAUNCHXL_UARTCOUNT
} CC2640R2_LAUNCHXL_UARTName;

/*!
 *  @def    CC2640R2_LAUNCHXL_UDMAName
 *  @brief  Enum of DMA buffers
 */
typedef enum CC2640R2_LAUNCHXL_UDMAName {
    CC2640R2_LAUNCHXL_UDMA0 = 0,

    CC2640R2_LAUNCHXL_UDMACOUNT
} CC2640R2_LAUNCHXL_UDMAName;

/*!
 *  @def    CC2640R2_LAUNCHXL_WatchdogName
 *  @brief  Enum of Watchdogs
 */
typedef enum CC2640R2_LAUNCHXL_WatchdogName {
    CC2640R2_LAUNCHXL_WATCHDOG0 = 0,

    CC2640R2_LAUNCHXL_WATCHDOGCOUNT
} CC2640R2_LAUNCHXL_WatchdogName;

/*!
 *  @def    CC2650_LAUNCHXL_TRNGName
 *  @brief  Enum of TRNG names on the board
 */
typedef enum CC2640R2_LAUNCHXL_TRNGName {
    CC2640R2_LAUNCHXL_TRNG0 = 0,
    CC2640R2_LAUNCHXL_TRNGCOUNT
} CC2640R2_LAUNCHXL_TRNGName;

#ifdef __cplusplus
}
#endif

#endif /* __CC2640R2_LAUNCHXL_BOARD_H__ */
