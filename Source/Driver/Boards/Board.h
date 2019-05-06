/*
 * Board.h
 *
 *  Created on: 2017Äê8ÔÂ10ÈÕ
 *      Author: blue.zhengxinkang
 */
/*******************************************************************************

 @file  board.h

 @brief This file is a simple gateway to include the appropriate board.h file
        which is located in a corresponding subdirectory relative to this file.
        In this way, all projects look the same and only need to include this
        board.h. To change a board, the project only need update the board
        define. Presently, the following board targets are possible:

        CC2640R2DK_CXS
        CC2640R2_LAUNCHXL

        If a project needs to change the board defined for a particular target,
        they can modify the board.h located in the corresponding board
        subdirectory.

        NOTE: THIS FILE SHOULD ONLY BE CHANGED TO ADD A NEW BOARD/TARGET!

 Group: CMCU, LPC, SCS
 Target Device: CC2640R2

 *******************************************************************************

 Copyright (c) 2015-2017, Texas Instruments Incorporated
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

 *******************************************************************************
 Release Name: simplelink_cc2640r2_sdk_1_30_00_25
 Release Date: 2017-03-02 20:08:35
 ******************************************************************************/
#ifndef SOURCE_DRIVERS_BOARD_BOARD_H_
#define SOURCE_DRIVERS_BOARD_BOARD_H_


#include "Config/ConfigProtocol.h"

#if     (BOARD_DEFINE == BOARD_DEFINE_BLEONLOCK_S31)
    #warning "BOARD_DEFINE  == BOARD_DEFINE_BLEONLOCK_S31"
    #include "BOARD_BLEONLOCK_4XS/Board.h"
    #include "BOARD_BLEONLOCK_4XS/ex_include_tirtos.h"
    #include "BOARD_BLEONLOCK_4XS/scif.h"
#elif   (BOARD_DEFINE == BOARD_DEFINE_BLEMODULE_PA)
    #warning "BOARD_DEFINE  == BOARD_DEFINE_BLEMODULE_PA"
    #include "BOARD_BLEMODULE_4XS/Board.h"
    #include "BOARD_BLEMODULE_4XS/ex_include_tirtos.h"
    #include "BOARD_BLEMODULE_4XS/scif.h"
#elif   (BOARD_DEFINE == BOARD_DEFINE_LANUCHPAD)
    #warning "BOARD_DEFINE  == BOARD_DEFINE_LANUCHPAD"
    #include "CC2640R2_BOARD_7XS/Board.h"
    #include "CC2640R2_BOARD_7XS/ex_include_tirtos.h"
    #include "CC2640R2_BOARD_7XS/scif.h"
#else
    #error "***ERROR*** BOARD_DEFINE not select!"
#endif

#if defined (CC2640R2DK_4XS)
    #warning "CHIP_PACKAGE  == CC2640R2DK_4XS"
#elif defined (CC2640R2DK_7XS)
    #warning "CHIP_PACKAGE  == CC2640R2DK_7XS"
#else
    #error "***ERROR*** Invalid CHIP_PACKAGE Specified! CHIP_PACKAGE not define!"
#endif


#endif /* SOURCE_DRIVERS_BOARD_BOARD_H_ */
