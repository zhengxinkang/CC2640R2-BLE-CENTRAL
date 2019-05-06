/**
 * @addtogroup module_Common
 * @{
 */

/**
 * @file
 * @brief �����и��ٵ�����Ϣ�����
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
 * @brief ���õ�����Ϣ�Ķ�̬�������
 * @param level ������룬��ֵԽ������ĵ�����Ϣ��Խ�ࡣ
 */
void Trace_SetLevel(unsigned char level)
{
    g_traceLevel = level;
}

/**
 * @brief ��ȡ������Ϣ���������
 * @return ������롣
 */
unsigned char Trace_GetLevel()
{
    return g_traceLevel;
}


#if CONSOLE_EMULATOR
/**
 * @def TRACE_LEVEL
 * @brief ��ָ���ļ��������Ϣ��
 * @details ʹ��ʱ����ָ���������ʹ�������Ƚ��鷳��������ʹ��һЩ�Զ��弶��
 * @note ������ʹ�øú�ֱ�������Ϣ����Ӧ�õ���ϵͳԤ�ȶ���õļ�����ΪֻҪ��
 *  @ref _CS_TRACE_LEVEL ���뿪�أ����ܼ�����ʲô�����궼�������룬�����С���ܿ��ơ�
 */
void TRACE_LEVEL(unsigned char level, const char *strFormat, ...)
{
    if ((level) <= g_traceLevel)
    {
        va_list args;
        uint8_t Length;

        /* Write out the Data.                                         */
        va_start(args, strFormat);
        ///@todo:�Ż���̫����ջ��ԴTBD
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
