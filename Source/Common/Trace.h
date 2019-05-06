/**
 * @page page_DebugTracer �������
 *
 * ʹ�� TRACE ϵ�к���԰���ͬ�ļ������������Ϣ���⽫��ֱ��ʹ�� Printf ���������
 * ϸ����ƴ�ӡ��Ϣ�͵�����Ϣ����һ�����ƴ���ĳߴ硣
 *
 * TRACE ϵ�к�����ڳ�������ʱ���������Ϣ������̨������֧���������
 * ����̬����Ͷ�̬���𣩣�ֻ�������������ĵ��òŻ���ֱ�����Ϣ��
 *
 * �����г���֧�ֵ� TRACE �꼰��Ӧ�ļ����壺
 * - @ref TRACE_FATAL �������������Ϣ������Ϊ�� @ref TRACE_LEVEL_FATAL
 * - @ref TRACE_ERROR ���������Ϣ������Ϊ�� @ref TRACE_LEVEL_ERROR
 * - @ref TRACE_WARN �������/�쳣��Ϣ������Ϊ�� @ref TRACE_LEVEL_WARNING
 * - @ref TRACE_INFO ���ҵ����Ϣ������Ϊ�� @ref TRACE_LEVEL_INFO
 * - @ref TRACE_DEBUG ���������Ϣ������Ϊ�� @ref TRACE_LEVEL_DEBUG
 * - @ref TRACE_MARK ������̱�ǣ�����Ϊ�� @ref TRACE_LEVEL_MARK
 * - @ref TRACE_CODE ��������ǣ�����Ϊ�� @ref TRACE_LEVEL_CODE
 *
 *
 * <br> <br>
 * @section _CS_TRACE_LEVEL ��̬�������
 *
 * ���뿪�غ꣺_CS_TRACE_LEVEL
 *
 * ���򷢲�ʱ����ͨ�����øñ��뿪�������徲̬���������ȥ������Ҫ�� TRACE ���Դ��룬
 * �����������С��ֻ�и��� _CS_TRACE_LEVEL ����� TRACE ��Żᱻ���롣
 *
 * δ���屾�����Ϊ0��ʾ�ر� TRACE ����Ϣ����������Ҫ����ú�ʱ��
 * ���븳��һ��������ֵ���Ա�ʾ�����Trace���𡣶������ֵԽ�������ϢԽ�࣬
 * ��������Ĵ���ҲԽ��ֻ�и��ڱ��궨��ļ���ʱ�� TRACE ����Żᱻ���롣
 * ������Ŀ�����ж���  _CS_TRACE_LEVEL ��ֵ����ָ����̬���Լ������δ����ú꣬
 * ���������� TRACE ����������롣
 *
 *
 * <br> <br>
 * @section sec_DebugTracer_Coding �����д
 *
 * ͷ�ļ������� Utils.h �� Trace.h
 *
 * ����ͷ�ļ��󣬾Ϳ���ʹ�� TRACE ϵ�к��ˡ�
 *
 *
 * <br> <br>
 * @section sec_DebugTracer_DynLevel ��̬�������
 *
 * �꿪�� _CS_TRACE_LEVEL ����ļ��������ھ�̬���𣬾�̬���������ˣ����������
 * �����Ƴ���ͻ����������롣����ʹ��̬�����������㣬Ҫ��ӡ����ϢҲ����û������������С�
 * ��Ϊ Trace ģ�鶨������һ��������𣺶�̬����Ĭ��Ϊ  @ref TRACE_LEVEL_INFO ��
 * ������Ȼ������������룬����̬���������������Ҳ������������� Trace_SetLevel()
 * �������ö�̬�������
 *
 * ���ʹ���˱�ģ���ṩ�������г��򣬿���ֱ���ڿ���̨��ִ�� trace ���������ö�̬����
 */


/**
 * @addtogroup module_Common
 * @{
 */

/**
 * @file
 * @brief �����и��ٵ�����Ϣ�����
 * @details ���ڱ�ģ���ʹ�ã���μ��� @ref page_DebugTracer
 * @note ��ģ��ʹ����C99��׼�Ŀɱ��������ƣ�����ĳЩ��������������Ҫ��GCC��չ����
 *  �����߼����á���֪ GCC �� MSVC8 ������ʹ�á�
 * @version 1.0.0
 * @author kim.xiejinqiang
 * @date 2013-4-9
 */


#ifndef COMMON_TRACE_H_
#define COMMON_TRACE_H_

#include "Console/Console.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Traceģ��ĵ�ǰ�������
 */
extern unsigned char g_traceLevel;

#define TRACE_LEVEL_DISABLE                 0   ///< ���ټ��𣺽�ֹ�κ����
#define TRACE_LEVEL_FATAL                   1   ///< ���ټ����������󣬳����޷���ȣ�ϵͳ������
#define TRACE_LEVEL_ERROR                   2   ///< ���ټ���һ�����ִ�н���Ǵ����
#define TRACE_LEVEL_WARNING                 3   ///< ���ټ��𣺾�����Ϣ��������ܳ���Ǳ������
#define TRACE_LEVEL_INFO                    4   ///< ���ټ���ҵ����Ϣ������ҵ���߼���ҵ������
#define TRACE_LEVEL_DEBUG                   5   ///< ���ټ��𣺵�����Ϣ����������ִ�й���
#define TRACE_LEVEL_MARK                    6   ///< ���ټ��𣺹��̱�ʶ����������λ��
#define TRACE_LEVEL_CODE                    7   ///< ���ټ��𣺴���ۼ�����������λ��

void TRACE_LEVEL(unsigned char level, const char *strFormat, ...);
void TRACE_HEXGROUP_LEVEL(unsigned char level,unsigned char* data, unsigned short len, char separator);
void TRACE_DATA_LEVEL(unsigned char level, unsigned char* data, unsigned short len);

#define DEBUG_STATE_MACHINE_BLE 1
//�����������
#if DEBUG_STATE_MACHINE_BLE
    #define TRACE_STATE_MACHINE_BLE_DEBUG(fmt, ...)     TRACE_DEBUG( fmt, ##__VA_ARGS__);
    #define TRACE_STATE_MACHINE_BLE_ERROR(fmt, ...)     TRACE_ERROR( fmt, ##__VA_ARGS__);
    #define TRACE_STATE_MACHINE_BLE(fmt, ...)           TRACE_DEBUG( fmt, ##__VA_ARGS__);
#else
    #define TRACE_STATE_MACHINE_BLE_DEBUG(fmt, ...)
    #define TRACE_STATE_MACHINE_BLE_ERROR(fmt, ...)
    #define TRACE_STATE_MACHINE_BLE(fmt, ...)
#endif

/**
 * @def TRACE_FATAL
 * @brief �������������Ϣ��
 * @details ͨ���ڳ�������޷���صĴ���ʱʹ�ã�ϵͳ���ٱ�����
 *  <p> ʹ�ø�ʽ�� printf ���ơ�
 */
#define TRACE_FATAL(fmt, ...)           TRACE_LEVEL(TRACE_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#define TRACE_FATA_HEXGROUP(_x,_y,_z)   TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_FATA)
#define TRACE_FATA_DATA(_x, _y)         TRACE_DATA_LEVEL(TRACE_LEVEL_FATA)

/**
 * @def TRACE_ERROR
 * @brief ���һ�������Ϣ��
 * @details ͨ���ڳ�����ִ���ʱʹ�ã��������ܼ������У�����������Ǵ���ġ�
 *  <p> ʹ�ø�ʽ�� printf ���ơ�
 */
#define TRACE_ERROR(fmt, ...)               TRACE_LEVEL(TRACE_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define TRACE_ERROR_HEXGROUP(_x,_y,_z)      TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_ERROR, _x,_y,_z)
#define TRACE_ERROR_DATA(_x, _y)            TRACE_DATA_LEVEL(TRACE_LEVEL_ERROR, _x, _y)

/**
 * @def TRACE_WARN
 * @brief ���������Ϣ��
 * @details ͨ���ڳ������Ԥ֪���쳣ʱʹ�ã�������ܳ���Ǳ�����⡣
 *  <p> ʹ�ø�ʽ�� printf ���ơ�
 */
#define TRACE_WARN(fmt, ...)                TRACE_LEVEL(TRACE_LEVEL_WARNING, fmt, ##__VA_ARGS__)
#define TRACE_WARN_HEXGROUP(_x,_y,_z)       TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_WARN, _x,_y,_z)
#define TRACE_WARN_DATA(_x, _y)             TRACE_DATA_LEVEL(TRACE_LEVEL_WARN, _x, _y)

/**
 * @def TRACE_INFO
 * @brief ���ҵ����Ϣ��
 * @details ����ҵ���߼���ҵ�����̣��û���ά����һ���ܿ�����
 *  <p> ʹ�ø�ʽ�� printf ���ơ�
 */
#define TRACE_INFO(fmt, ...)                TRACE_LEVEL(TRACE_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define TRACE_INFO_HEXGROUP(_x,_y,_z)       TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_INFO, _x,_y,_z)
#define TRACE_INFO_DATA(_x, _y)             TRACE_DATA_LEVEL(TRACE_LEVEL_INFO, _x, _y)

/**
 * @def TRACE_DEBUG
 * @brief ���������Ϣ��
 * @details ��������ִ�й��̻��м��������������ʹ�á�
 *  <p> ʹ�ø�ʽ�� printf ���ơ�
 */
#define TRACE_DEBUG(fmt, ...)               TRACE_LEVEL(TRACE_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define TRACE_DEBUG_HEXGROUP(_x,_y,_z)      TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_DEBUG, _x,_y,_z)
#define TRACE_DEBUG_DATA(_x, _y)            TRACE_DATA_LEVEL(TRACE_LEVEL_DEBUG, _x, _y)

/**
 * @def TRACE_MARK
 * @brief ������̱�ʶ��Ϣ��
 * @details ��������ִ�й켣�����������ʹ�á���չ���𣬲�����ʹ�á�
 *  <p> ʹ�ø�ʽ�� printf ���ơ�
 * @note ��Ҫ�򿪺���뿪�� @ref _CS_TRACE_LEVEL ��
 */

#define TRACE_MARK(fmt, ...)                TRACE_LEVEL(TRACE_LEVEL_MARK, fmt, ##__VA_ARGS__)
#define TRACE_MARK_HEXGROUP(_x,_y,_z)       TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_MARK, _x,_y,_z)
#define TRACE_MARK_DATA(_x, _y)             TRACE_DATA_LEVEL(TRACE_LEVEL_MARK, _x, _y)


/**
 * @def TRACE_CODE
 * @brief ���������ٺۼ���
 * @details ��������λ�ã����������ʹ�á���չ���𣬲�����ʹ�á�
 *  <p> ʹ�ø�ʽ�� printf ���ơ�
 * @note ��Ҫ�򿪺���뿪�� @ref _CS_TRACE_LEVEL ��
 */

#define TRACE_CODE(fmt, ...)                TRACE_LEVEL(TRACE_LEVEL_CODE, fmt, ##__VA_ARGS__)
#define TRACE_CODE_HEXGROUP(_x,_y,_z)       TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_CODE,_x,_y,_z)
#define TRACE_CODE_DATA(_x, _y)             TRACE_DATA_LEVEL(TRACE_LEVEL_CODE,_x, _y)


void Trace_SetLevel(unsigned char level);
unsigned char Trace_GetLevel();


#ifdef __cplusplus
}
#endif

#endif // COMMON_TRACE_H_

/** @} */
