/**
 * @addtogroup module_Common
 * @{
 */

/**
 * @file
 * @brief 命令行跟踪调试信息输出。
 * @details
 * @version 1.0.0
 * @author kim.xiejinqiang
 * @date 2013-4-8
 */

#include "Trace.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <StrLine.h>
#include "ConfigProtocol.h"

unsigned char g_traceLevel = _CS_TRACE_LEVEL;

/**
 * @brief 设置调试信息的动态输出级别。
 * @param level 级别代码，数值越大，输出的调试信息将越多。
 */
void Trace_SetLevel(unsigned char level)
{
    g_traceLevel = level;
}

/**
 * @brief 获取调试信息的输出级别。
 * @return 级别代码。
 */
unsigned char Trace_GetLevel()
{
    return g_traceLevel;
}


#if CONSOLE_EMULATOR
/**
 * @def TRACE_LEVEL
 * @brief 以指定的级别输出信息。
 * @details 使用时必须指定输出级别，使用起来比较麻烦，但可以使用一些自定义级别。
 * @note 不建议使用该宏直接输出信息，而应该调用系统预先定义好的级别，因为只要打开
 *  @ref _CS_TRACE_LEVEL 编译开关，不管级别是什么，本宏都将被编译，代码大小不受控制。
 */
void TRACE_LEVEL(unsigned char level, const char *strFormat, ...)
{
    if ((level) <= g_traceLevel)
    {
        va_list args;
        uint8_t Length;

        /* Write out the Data.                                         */
        va_start(args, strFormat);
        ///@todo:优化，太消耗栈资源TBD
        Length = vsnprintf((char*)s_bufTx, (sizeof(s_bufTx) - 1), strFormat, args);
        va_end(args);
        StrLine_WriteData((uint8_t*)s_bufTx, Length);
    }
    else
    {
        return;
    }
}

void TRACE_HEXGROUP_LEVEL(unsigned char level,unsigned char* data, unsigned short len, char separator)
{
    if ((level) <= g_traceLevel)
    {
        Console_HexGrounpOut(data, len, separator);
    }
    else
    {
        return;
    }
}

void TRACE_DATA_LEVEL(unsigned char level, unsigned char* data, unsigned short len)
{
    if ((level) <= g_traceLevel)
    {
        Console_OutData(data, len);
    }
    else
    {
        return;
    }
}
#else
void TRACE_LEVEL(unsigned char level, const char *strFormat, ...)
{
    return;
}

void TRACE_HEXGROUP_LEVEL(unsigned char level,unsigned char* data, unsigned short len, char separator)
{
    return;
}

void TRACE_DATA_LEVEL(unsigned char level, unsigned char* data, unsigned short len)
{
    return;
}
#endif
/** @} */
