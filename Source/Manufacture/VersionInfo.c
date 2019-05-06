/*
 * VersionInfo.c
 *
 *  Created on: 2018��10��19��
 *      Author: zheng
 */

/**
 * @file
 * @brief ��Ʒ�İ汾�����Ϣ��
 * @details �ṩ����汾�š�
 * @version 1.0.1
 * @author zheng
 * @date 2016-5-25
 */
#include "VersionInfo.h"
#include "ConfigProtocol.h"

//------S31�����ϵ�����-----------
/**

//---------------------------------------------------------
V1.0.0.1
֣�¿�
����ʱ�䣺18-10-21   11:16

����˵����
1������launchPadӲ��������һ��ʹ�ð汾��

*/
static const ManufVersion s_kSoftwareVersion =
{
        1,      // ���汾��
        0,      // �ΰ汾��
        0,      // �޶��汾��
        1       // ����汾��
};

//Ӳ���汾��
/**
//---------------------------------------------------------
V1.0.0.0
֣�¿�
����ʱ�䣺18-10-21   11:16

����˵����
1��launchPadӲ��-cc2640r2

*/
static const ManufVersion s_kHardwareVersion =
{
        1,      // ���汾��
        0,      // �ΰ汾��
        0,      // �޶��汾��
        0       // ����汾��
};

/**
 * @brief ��ȡ����汾�š�
 * @return ����汾�š�
 */
ManufVersion VersionInfo_GetSoftwareVersion(void)
{
    return s_kSoftwareVersion;
}


ManufVersion VersionInfo_GetHardwareVersion(void)
{
    return s_kHardwareVersion;
}

