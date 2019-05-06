/*
 * VersionInfo.h
 *
 *  Created on: 2017年8月10日
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_MANUFACTURE_VERSIONINFO_H_
#define SOURCE_MANUFACTURE_VERSIONINFO_H_


#include "Common\Types.h"

typedef struct ManufVersionStruct
{
    unsigned char major;       ///< 主版本号
    unsigned char minor;       ///< 次版本号
    unsigned char revision;    ///< 修订版本号
    unsigned char build;       ///< 编译版本号
}
ManufVersion;

extern ManufVersion VersionInfo_GetSoftwareVersion(void);
extern ManufVersion VersionInfo_GetHardwareVersion(void);


#endif /* SOURCE_MANUFACTURE_VERSIONINFO_H_ */
