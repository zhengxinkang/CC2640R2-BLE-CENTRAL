/*
 * VersionInfo.c
 *
 *  Created on: 2017年8月10日
 *      Author: blue.zhengxinkang
 */

#include "VersionInfo.h"
//#include "ConfigProtocol.h"

#if (SOFT_VERSION == SLOCKV4_BB_DNCP_SOFT_VERSION)
    //#error"WA: SOFT_VERSION == DNCP_SOFT_VERSION"
    static const ManufVersion s_kSoftwareVersion =
    {
     1,      // 主版本号
     0,      // 次版本号
     0,      // 修订版本号
     0       // 编译版本号
    };

#else
    #error "***ERROR*** soft version not define!"
#endif


/**
 * @brief 获取软件版本号。
 * @return 软件版本号。
 */
ManufVersion VersionInfo_GetSoftwareVersion(void)
{
    return s_kSoftwareVersion;
}

/**
 * @brief 获取硬件（电路）版本号。
 * @return 硬件版本号。
 */
ManufVersion VersionInfo_GetHardwareVersion(void)
{
    ManufVersion pcbVersion = {0};
    //pcbVersion.major = HardVersion_GetPcbVersion();
    //pcbVersion.minor = HardVersion_GetBomVersion();
    return pcbVersion;
}
