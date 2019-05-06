/*
 * VersionInfo.h
 *
 *  Created on: 2017��8��10��
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_MANUFACTURE_VERSIONINFO_H_
#define SOURCE_MANUFACTURE_VERSIONINFO_H_


#include "Common\Types.h"

typedef struct ManufVersionStruct
{
    unsigned char major;       ///< ���汾��
    unsigned char minor;       ///< �ΰ汾��
    unsigned char revision;    ///< �޶��汾��
    unsigned char build;       ///< ����汾��
}
ManufVersion;

extern ManufVersion VersionInfo_GetSoftwareVersion(void);
extern ManufVersion VersionInfo_GetHardwareVersion(void);


#endif /* SOURCE_MANUFACTURE_VERSIONINFO_H_ */
