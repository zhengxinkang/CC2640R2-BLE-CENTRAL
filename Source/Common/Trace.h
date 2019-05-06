/**
 * @page page_DebugTracer 调试输出
 *
 * 使用 TRACE 系列宏可以按不同的级别输出跟踪信息，这将比直接使用 Printf 更方便和灵活，
 * 细腻控制打印信息和调试信息，进一步控制代码的尺寸。
 *
 * TRACE 系列宏可以在程序运行时输出调试信息到控制台，并且支持输出级别
 * （静态级别和动态级别），只有满足输出级别的调用才会正直输出信息。
 *
 * 下面列出了支持的 TRACE 宏及相应的级别定义：
 * - @ref TRACE_FATAL 输出致命错误信息，级别为： @ref TRACE_LEVEL_FATAL
 * - @ref TRACE_ERROR 输出错误信息，级别为： @ref TRACE_LEVEL_ERROR
 * - @ref TRACE_WARN 输出警告/异常信息，级别为： @ref TRACE_LEVEL_WARNING
 * - @ref TRACE_INFO 输出业务信息，级别为： @ref TRACE_LEVEL_INFO
 * - @ref TRACE_DEBUG 输出调试信息，级别为： @ref TRACE_LEVEL_DEBUG
 * - @ref TRACE_MARK 输出过程标记，级别为： @ref TRACE_LEVEL_MARK
 * - @ref TRACE_CODE 输出代码标记，级别为： @ref TRACE_LEVEL_CODE
 *
 *
 * <br> <br>
 * @section _CS_TRACE_LEVEL 静态输出级别
 *
 * 编译开关宏：_CS_TRACE_LEVEL
 *
 * 程序发布时可以通过配置该编译开关来定义静态输出级别，以去除不必要的 TRACE 调试代码，
 * 以缩减代码大小。只有高于 _CS_TRACE_LEVEL 级别的 TRACE 宏才会被编译。
 *
 * 未定义本宏或定义为0表示关闭 TRACE 宏信息的输出。如果要定义该宏时，
 * 必须赋于一个级别数值，以表示具体的Trace级别。定义的数值越大输出信息越多，
 * 编译输出的代码也越大。只有高于本宏定义的级别时， TRACE 代码才会被编译。
 * 可在项目属性中定义  _CS_TRACE_LEVEL 的值，来指定静态调试级别。如果未定义该宏，
 * 将禁用所有 TRACE 调试输出代码。
 *
 *
 * <br> <br>
 * @section sec_DebugTracer_Coding 代码编写
 *
 * 头文件包含： Utils.h 或 Trace.h
 *
 * 引入头文件后，就可以使用 TRACE 系列宏了。
 *
 *
 * <br> <br>
 * @section sec_DebugTracer_DynLevel 动态输出级别
 *
 * 宏开关 _CS_TRACE_LEVEL 定义的级别是属于静态级别，静态级别满足了，编译出来的
 * 二进制程序就会包含输出代码。但即使静态级别条件满足，要打印的信息也可能没有输出到命令行。
 * 因为 Trace 模块定义了另一种输出级别：动态级别，默认为  @ref TRACE_LEVEL_INFO ，
 * 所以虽然包含了输出代码，但动态级别的条件不满足也不会输出。调用 Trace_SetLevel()
 * 可以设置动态输出级别。
 *
 * 如果使用了本模块提供的命令行程序，可以直接在控制台中执行 trace 命令来设置动态级别。
 */


/**
 * @addtogroup module_Common
 * @{
 */

/**
 * @file
 * @brief 命令行跟踪调试信息输出。
 * @details 关于本模块的使用，请参见： @ref page_DebugTracer
 * @note 本模块使用了C99标准的可变参数宏机制，对于某些编译器，可能需要打开GCC扩展或者
 *  其它高级配置。已知 GCC 和 MSVC8 可正常使用。
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
 * @brief Trace模块的当前输出级别。
 */
extern unsigned char g_traceLevel;

#define TRACE_LEVEL_DISABLE                 0   ///< 跟踪级别：禁止任何输出
#define TRACE_LEVEL_FATAL                   1   ///< 跟踪级别：致命错误，程序无法挽救，系统将崩溃
#define TRACE_LEVEL_ERROR                   2   ///< 跟踪级别：一般错误，执行结果是错误的
#define TRACE_LEVEL_WARNING                 3   ///< 跟踪级别：警告信息，程序可能出现潜在问题
#define TRACE_LEVEL_INFO                    4   ///< 跟踪级别：业务信息，表征业务逻辑或业务流程
#define TRACE_LEVEL_DEBUG                   5   ///< 跟踪级别：调试信息，表征程序执行过程
#define TRACE_LEVEL_MARK                    6   ///< 跟踪级别：过程标识，表征函数位置
#define TRACE_LEVEL_CODE                    7   ///< 跟踪级别：代码痕迹，表征代码位置

void TRACE_LEVEL(unsigned char level, const char *strFormat, ...);
void TRACE_HEXGROUP_LEVEL(unsigned char level,unsigned char* data, unsigned short len, char separator);
void TRACE_DATA_LEVEL(unsigned char level, unsigned char* data, unsigned short len);

#define DEBUG_STATE_MACHINE_BLE 1
//定义输出内容
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
 * @brief 输出致命错误信息。
 * @details 通常在程序出现无法挽回的错误时使用，系统面临崩溃。
 *  <p> 使用格式与 printf 相似。
 */
#define TRACE_FATAL(fmt, ...)           TRACE_LEVEL(TRACE_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#define TRACE_FATA_HEXGROUP(_x,_y,_z)   TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_FATA)
#define TRACE_FATA_DATA(_x, _y)         TRACE_DATA_LEVEL(TRACE_LEVEL_FATA)

/**
 * @def TRACE_ERROR
 * @brief 输出一般错误信息。
 * @details 通常在程序出现错误时使用，程序仍能继续运行，但结果可能是错误的。
 *  <p> 使用格式与 printf 相似。
 */
#define TRACE_ERROR(fmt, ...)               TRACE_LEVEL(TRACE_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define TRACE_ERROR_HEXGROUP(_x,_y,_z)      TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_ERROR, _x,_y,_z)
#define TRACE_ERROR_DATA(_x, _y)            TRACE_DATA_LEVEL(TRACE_LEVEL_ERROR, _x, _y)

/**
 * @def TRACE_WARN
 * @brief 输出警告信息。
 * @details 通常在程序出错预知的异常时使用，程序可能出现潜在问题。
 *  <p> 使用格式与 printf 相似。
 */
#define TRACE_WARN(fmt, ...)                TRACE_LEVEL(TRACE_LEVEL_WARNING, fmt, ##__VA_ARGS__)
#define TRACE_WARN_HEXGROUP(_x,_y,_z)       TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_WARN, _x,_y,_z)
#define TRACE_WARN_DATA(_x, _y)             TRACE_DATA_LEVEL(TRACE_LEVEL_WARN, _x, _y)

/**
 * @def TRACE_INFO
 * @brief 输出业务信息。
 * @details 描述业务逻辑或业务流程，用户或维护者一般能看懂。
 *  <p> 使用格式与 printf 相似。
 */
#define TRACE_INFO(fmt, ...)                TRACE_LEVEL(TRACE_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define TRACE_INFO_HEXGROUP(_x,_y,_z)       TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_INFO, _x,_y,_z)
#define TRACE_INFO_DATA(_x, _y)             TRACE_DATA_LEVEL(TRACE_LEVEL_INFO, _x, _y)

/**
 * @def TRACE_DEBUG
 * @brief 输出调试信息。
 * @details 描述程序执行过程或中间结果，供程序调试使用。
 *  <p> 使用格式与 printf 相似。
 */
#define TRACE_DEBUG(fmt, ...)               TRACE_LEVEL(TRACE_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define TRACE_DEBUG_HEXGROUP(_x,_y,_z)      TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_DEBUG, _x,_y,_z)
#define TRACE_DEBUG_DATA(_x, _y)            TRACE_DATA_LEVEL(TRACE_LEVEL_DEBUG, _x, _y)

/**
 * @def TRACE_MARK
 * @brief 输出过程标识信息。
 * @details 描述函数执行轨迹，供程序调试使用。扩展级别，不建议使用。
 *  <p> 使用格式与 printf 相似。
 * @note 需要打开宏编译开关 @ref _CS_TRACE_LEVEL 。
 */

#define TRACE_MARK(fmt, ...)                TRACE_LEVEL(TRACE_LEVEL_MARK, fmt, ##__VA_ARGS__)
#define TRACE_MARK_HEXGROUP(_x,_y,_z)       TRACE_HEXGROUP_LEVEL(TRACE_LEVEL_MARK, _x,_y,_z)
#define TRACE_MARK_DATA(_x, _y)             TRACE_DATA_LEVEL(TRACE_LEVEL_MARK, _x, _y)


/**
 * @def TRACE_CODE
 * @brief 输出代码跟踪痕迹。
 * @details 描述代码位置，供程序调试使用。扩展级别，不建议使用。
 *  <p> 使用格式与 printf 相似。
 * @note 需要打开宏编译开关 @ref _CS_TRACE_LEVEL 。
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
