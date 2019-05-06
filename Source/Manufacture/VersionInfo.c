/*
 * VersionInfo.c
 *
 *  Created on: 2018年10月19日
 *      Author: zheng
 */

/**
 * @file
 * @brief 产品的版本相关信息。
 * @details 提供软件版本号。
 * @version 1.0.1
 * @author zheng
 * @date 2016-5-25
 */
#include "VersionInfo.h"
#include "ConfigProtocol.h"

//------S31锁板上的蓝牙-----------
/**

//---------------------------------------------------------
V1.0.0.1
郑新康
更新时间：18-10-21   11:16

更新说明：
1、基于launchPad硬件发布第一个使用版本。

*/
static const ManufVersion s_kSoftwareVersion =
{
        1,      // 主版本号
        0,      // 次版本号
        0,      // 修订版本号
        1       // 编译版本号
};

//硬件版本号
/**
//---------------------------------------------------------
V1.0.0.0
郑新康
更新时间：18-10-21   11:16

更新说明：
1、launchPad硬件-cc2640r2

*/
static const ManufVersion s_kHardwareVersion =
{
        1,      // 主版本号
        0,      // 次版本号
        0,      // 修订版本号
        0       // 编译版本号
};

/**
 * @brief 获取软件版本号。
 * @return 软件版本号。
 */
ManufVersion VersionInfo_GetSoftwareVersion(void)
{
    return s_kSoftwareVersion;
}


ManufVersion VersionInfo_GetHardwareVersion(void)
{
    return s_kHardwareVersion;
}

