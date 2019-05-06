/*
 * ConsoleConfig.h
 *
 *  Created on: 2017年8月9日
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_CONSOLE_CONSOLECONFIG_H_
#define SOURCE_CONSOLE_CONSOLECONFIG_H_


// 请根据项目情况和芯片资源，适当更改下面的宏定义。
// 如果尚未弄懂相关宏的意思，请保持默认。

/**
 * @brief 定义命令提示符字串。
 * @details 该提示符将打印在每行行头，以提示用户键入命令。
 */
#define CONSOLE_PROMPT_STRING               "<Lock>"


/**
 * @brief 定义是否需要在初始化时打印欢迎信息。
 * @details 1 为需要打印，0 为不需要打印。
 *  如果需要显示欢迎信息，还需要在下面定义具体的欢迎信息；
 *  不需打印的可忽略具体信息的定义。
 */
#define CONSOLE_NEED_WELCOME_MSG            1


#ifdef CONSOLE_NEED_WELCOME_MSG

/**
 * @brief 定义欢迎信息中的产品信息。
 */
#define CONSOLE_WELCOME_PRODUCT             "Embedded"

/**
 * @brief 定义欢迎信息中的版权信息。
 * @details 如不需要，请注释该宏。
 */
#define CONSOLE_WELCOME_COPYRIGHT           "Copyright(C) Stag TI-RTOS 2017-2018"

/**
 * @brief 定义欢迎信息中的其它信息。
 * @details 如不需要，请注释该宏。
 */
#define CONSOLE_WELCOME_OTHER               "EmConsole for cc2640r2 port"

/**
 * @brief 定义是否在欢迎信息中打印版本信息。
 * @details 1 为需要打印，0 为不需要打印。
 *  命令行的版本号将在 CmdLine 实现中定义。
 */
#define CONSOLE_WELCOME_NEED_VERSION        1

#endif  // CONSOLE_NEED_WELCOME_MSG

/**
 * @brief 定义命令行缓冲大小。
 * @details 该值将决定一行命令文本所能支持的最大字符数目。
 */
#define CONSOLE_LINE_BUF_SIZE               64

/**
 * @brief 定义命令行支持的历史命令条数。
 * @details 该值将决定上下箭头翻看的命令数目，必须大于等于1。
 * @note 最大只支持256条历史命令
 */
#define CONSOLE_CMD_HISTORY_NUM             2

/**
 * @brief 定义驱动接收缓冲大小。
 * @details 该值将决定一行命令文本所能支持的最大字符数目。
 * @note 接收缓冲将由Console模块实现，而不是ConsoleDriver。
 */
#define CONSOLE_RX_BUF_SIZE                 255

/**
 * @brief 定义驱动发送缓冲大小。
 * @details 发送缓冲越大，一次性发送的字符越多。
 *  在实时（如通信）调试时，较大的发送缓冲有助于一次性打印较多的字符。
 *  <p> 在打印大量数据时（如 help 命令列出所有支持的命令），显示的内容可能不完整，
 *  后面部分经常被截断，这通常是因为发送缓冲区设置得过小的原因，解决的办法有：
 *  - 调大发送缓冲，即把本宏设置得大一点。通常受限于设备的RAM资源。
 *  - 在打印大文本段时，拆分为多条打印语句，打印语句间加延时。
 *
 * @note 该值是否被采用，取决于具体的ConsoleDriver的实现。
 *  本库提供的所有ConsoleDriver，默认将采用本处定义的值。
 */
#define CONSOLE_TX_BUF_SIZE                 256//输出hexgroup的时候,需要用到很大的空间

/**
 * @brief 定义控制台命令支持的最大参数个数。
 * @details 该值将决定应用命令最多能接收的参数个数。
 */
#define CONSOLE_MAX_ARGUMENTS               16


#endif /* SOURCE_CONSOLE_CONSOLECONFIG_H_ */
