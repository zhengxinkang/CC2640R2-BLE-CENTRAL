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
V2.0.0.1
更新说明：
1、修复显示跑动问题，任务优先级修改。
2、增加休眠功耗测试失败重试机制，设置休眠功耗测试的上下限。
3、增加蓝牙测试失败重试机制。
4、增加测试通过，显示休眠电流。
5、其他修改。

//---------------------------------------------------------
V2.0.0.0
更新说明：
1、基于（老化/检具）两用硬件发布的第一个版本（软件检具用）

//---------------------------------------------------------
V1.0.0.1
郑新康
更新时间：18-10-21   11:16

*/
static const ManufVersion s_kSoftwareVersion =
{
        2,      // 主版本号
        0,      // 次版本号
        2,      // 修订版本号
        0       // 编译版本号
};

//硬件版本号
/**
//---------------------------------------------------------
V2.0.0.0
更新说明：
1、老化/检具 两用板V2.0.0.0

//---------------------------------------------------------
V1.0.0.0
郑新康
更新时间：18-10-21   11:16

*/
static const ManufVersion s_kHardwareVersion =
{
        2,      // 主版本号
        0,      // 次版本号
        0,      // 修订版本号
        1       // 编译版本号
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

