/*
 * Console.h
 *
 *  Created on: 2017年8月9日
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_CONSOLE_CONSOLE_H_
#define SOURCE_CONSOLE_CONSOLE_H_


#include <stdint.h>
#include "ConsoleConfig.h"
// 初始化
extern void Console_Init(void);

// 后台任务
extern int Console_RoutineHandle(void);

// 输出信息到控制台
extern void Console_Prompt(void);
extern void Console_Welcome(void);

// 格式化输出函数接口
extern void Console_Out(const char * strFormat, ...);

//连续以字符串形式输出16进制数据
extern void Console_HexGrounpOut(uint8_t* hexGroup, uint8_t len, char separator);

// 数据直接输出
//extern void Console_OutString(const char * str);
extern void Console_OutData(uint8_t* str, uint8_t len);
extern uint8_t s_bufTx[CONSOLE_TX_BUF_SIZE];
//*******************************************************************
// 输出函数接口宏

/**
 * @brief 一个简单实现的格式化输出函数。
 * @details 本宏是函数 @ref Console_Out() 的宏定义。
 */
#define ConsoleOut          Console_Out

/**
 * @brief 一个简单实现的格式化输出函数。
 * @details 本宏是函数 Console_Out() 的宏定义。
 */
#define EM_TRACE            Console_Out


//*******************************************************************
// 命令处理函数和命令表定义

/**
 * @brief 接口函数原型定义：控制台命令处理函数。
 * @param argc 参数个数。
 * @param argv 命令参数列表。
 *  argv[0]为命令关键字本身，argv[1]开始是命令的参数字串。
 * @return 命令执行状态，0 表示成功执行，非 0 为异常，参见 @ref ConsoleCmdRetCode
 */
typedef int (*IfConsoleCommandHandle)(int argc, char *argv[]);


/**
 * @brief 命令表表项结点类型。
 */
typedef struct
{
    const char * cmdKeyword;            ///< 命令关键字（字符串）
    IfConsoleCommandHandle cmdHandle;   ///< 命令处理函数
    const char * cmdHelp;               ///< 命令说明文本
}
CmdLineEntry;


/**
 * @brief 命令表，保存命令关键字与其处理函数之间的对应关系。
 * @details 具体命令表由用户接口模块 @ref CmdLine.c 中定义。
 * @note 应用程序必须提供该表。
 */
extern const CmdLineEntry g_kConsoleCmdTable[];


//*******************************************************************
// 命令行版本管理

/**
 * @brief 命令行版本号
 */
typedef struct
{
    unsigned short major;       ///< 主版本号
    unsigned short minor;       ///< 次版本号
    unsigned short revision;    ///< 修订版本号
    unsigned short build;       ///< 编译版本号
}
CmdLineVersion;

/**
 * @brief 命令行版本号。
 * @note 应用程序（ @ref CmdLine.c ）必须定义该版本号，并且根据需要更新。
 */
extern const CmdLineVersion g_kCmdLineVersion;


//*******************************************************************
// 返回结果码定义

/**
 * @brief 返回结果码定义
 */
typedef enum ConsoleCmdRetCodeEnum
{
    CCRC_OK = 0,
    CCRC_INVALID_COMMAND,
    CCRC_INVALID_PARAM,
    CCRC_NOT_READY,
    CCRC_NO_FILE,
    CCRC_NO_PATH,
    CCRC_INVALID_NAME,
    CCRC_INVALID_DRIVE,
    CCRC_DENIED,
    CCRC_EXIST,
    CCRC_RW_ERROR,
    CCRC_WRITE_PROTECTED,
    CCRC_NOT_ENABLED,
    CCRC_NO_FILESYSTEM,
    CCRC_INVALID_OBJECT,
    CCRC_TIMEOUT,
    CCRC_OMIT_PROMPT,
    CCRC_ERROR_COM,
    CCRC_ERROR_PACKAGE,
    CCRC_MKFS_ABORTED,
    CCRC_CMD_TERMINATE
}
ConsoleCmdRetCode;


#endif /* SOURCE_CONSOLE_CONSOLE_H_ */
