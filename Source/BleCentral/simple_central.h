/******************************************************************************

 @file       simple_central.h

 @brief This file contains the Simple Central sample application
        definitions and prototypes.

 Group: CMCU, SCS
 Target Device: CC2640R2

 ******************************************************************************
 
 Copyright (c) 2013-2017, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 Release Name: simplelink_cc2640r2_sdk_01_50_00_58
 Release Date: 2017-10-17 18:09:51
 *****************************************************************************/

#ifndef SIMPLEBLECENTRAL_H
#define SIMPLEBLECENTRAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "Types.h"

typedef enum{
    RET_FIND_DEVICE_INLIST_OK               = 0,    //找到设备
    RET_FIND_DEVICE_INLIST_ERROR_SCANNING   = 1,    //扫描中
    RET_FIND_DEVICE_INLIST_ERROR_NODEVICE   = 2,    //列表中没有设备
    RET_FIND_DEVICE_INLIST_ERROR_NOFIND     = 3,    //未找到该设备
}RET_FIND_DEVICE_INLIST;

typedef enum{
    RET_CONNECT_OK                          = 0,    //成功开始连接
    RET_CONNECT_ERROR_NODEVICE              = 1,    //没有设备
    RET_CONNECT_ERROR_SCANNING              = 2,    //扫描中
}RET_CONNECT;

typedef enum{
    RET_DISCOVER_OK                         = 0,
    RET_DISCOVER_ERROR_BLE_STATE            = 1,    //ble状态错误
    RET_DISCOVER_ERROR_SCAN_RESULT          = 2,    //已经有扫描结果
}RET_DISCOVER;
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */
/*
 * Task creation function for the Simple Central.
 */
extern void SimpleBLECentral_createTask(void);
extern void MyEvent_post(UArg arg);
void simpleCentral_setSearchDeviceAddr(uint8_t* searchAddr);
extern void Timer_sendDataHandle(UArg arg);
extern void MyEvent_sendDataHandle();
void simpleCentral_action();


extern void             BleTest_writeData();
extern void             BleTest_readData();
extern bool             BleTest_finish(bool isSuccess);

RET_FIND_DEVICE_INLIST  BleTest_findDeviceInList();
RET_CONNECT             BleTest_connect();
RET_DISCOVER            BleTest_discover();
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEBLECENTRAL_H */
