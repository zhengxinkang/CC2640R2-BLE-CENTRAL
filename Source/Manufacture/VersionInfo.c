/*
 * VersionInfo.c
 *
 *  Created on: 2017��8��10��
 *      Author: blue.zhengxinkang
 */

#include "VersionInfo.h"
//#include "ConfigProtocol.h"

#if (SOFT_VERSION == SLOCKV4_BB_DNCP_SOFT_VERSION)
    //#error"WA: SOFT_VERSION == DNCP_SOFT_VERSION"
    static const ManufVersion s_kSoftwareVersion =
    {
     1,      // ���汾��
     0,      // �ΰ汾��
     0,      // �޶��汾��
     0       // ����汾��
    };

#else
    #error "***ERROR*** soft version not define!"
#endif


/**
 * @brief ��ȡ����汾�š�
 * @return ����汾�š�
 */
ManufVersion VersionInfo_GetSoftwareVersion(void)
{
    return s_kSoftwareVersion;
}

/**
 * @brief ��ȡӲ������·���汾�š�
 * @return Ӳ���汾�š�
 */
ManufVersion VersionInfo_GetHardwareVersion(void)
{
    ManufVersion pcbVersion = {0};
    //pcbVersion.major = HardVersion_GetPcbVersion();
    //pcbVersion.minor = HardVersion_GetBomVersion();
    return pcbVersion;
}
