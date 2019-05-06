/*
 * Console.c
 *
 *  Created on: 2017年8月9日
 *      Author: blue.zhengxinkang
 */

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <StrLine.h>

#include "Console.h"
#include "Common\StringExchange.h"
#include "Trace.h"
#include "CmdLine.h"
#include "Manufacture/VersionInfo.h"
#include "Config/ConfigProtocol.h"
//************************************
// 命令行解析状态
#define CONSOLE_EVENT_NONE              0   // 暂无命令
#define CONSOLE_EVENT_NEW_CMD           1   // 新命令
#define CONSOLE_EVENT_TERMINATE         2   // 终止命令，结束运行当前命令
static volatile unsigned char s_consoleEvent = CONSOLE_EVENT_NONE;

// 命令行解析结果
#define CMDLINE_BAD_CMD         (-1)    // 系统不支持的命令（未定义）
#define CMDLINE_TOO_MANY_ARGS   (-2)    // 输入的命令行参数太多
#define CMDLINE_NO_CMD          (-3)    // 无命令

//************************************
// 命令行接收缓冲和写索引
uint8_t s_bufTx[CONSOLE_TX_BUF_SIZE];
static void Console_scanf(uint8_t *string, uint16_t len);

/**
 * @brief 控制台初始化。
 */
void Console_Init(void)
{
    StrLine_init();
    StrLine_recieveToUp_setFunc(Console_scanf);
    Console_Welcome();
}

/**
 * @brief 解析命令行文本并执行命令。
 * @param cmdLine 命令行文本字符串。
 * @return 解析或执行状态。
 */
int Console_findCmd(int argc, char *argv[]);
//输入字符串-转化为参数
void Console_scanf(uint8_t *string, uint16_t len)
{
    static char *argv[CONSOLE_MAX_ARGUMENTS + 1];
    char * pos;
    int argc;
    int bFindArg = 1;
    uint8_t char_index=0;

    // 初始化各参数为空，方便应用命令解析
    for (argc = 0; argc <= CONSOLE_MAX_ARGUMENTS; argc++)
        argv[argc] = 0;

    // 开始解析命令行
    argc = 0;
    pos = (char*)string;

    // 解析整行文本直到遇到空字符，表示行文本结束
    for(char_index=0; char_index<len; char_index++)
    {
        // 空白字符是一个参数的 “前导符”
        if(*pos == ' ' || *pos == '\t')
        {
            *pos = 0;
            bFindArg = 1;
        }
        else
        {
            // 有非空白字符，一个参数开始
            // 上一次有找到过参数
            if(bFindArg)
            {
                if(argc < CONSOLE_MAX_ARGUMENTS)
                {
                    // 记录该参数
                    argv[argc] = pos;
                    argc++;
                    bFindArg = 0;
                }
                else
                {
                    // 参数过多，停止解析
                    return;
                }
            }
        }

        pos++;
    }

    // 有解析到参数；第一个参数即是命令关键字
    if(argc)
    {
        Console_findCmd(argc, argv);
    }
}

/*****************************************
根据参数解析，寻找对应的命令
******************************************/
int Console_findCmd(int argc, char *argv[])
{
    CmdLineEntry *cmdEntry;
    // 遍历整个命令表，匹配关键字
    cmdEntry = (CmdLineEntry *) &g_kConsoleCmdTable[0];
    TRACE_CODE("CONSOLE INPUT CMD: ");
    Cmd_showparam(argc, argv);
    while(cmdEntry->cmdKeyword)
    {
        // 匹配命令关键字，调用相应的命令处理函数
        if(!strcmp(argv[0], cmdEntry->cmdKeyword))
        {
            return(cmdEntry->cmdHandle(argc, argv));
        }
        cmdEntry++;
    }
    TRACE_ERROR("Console.c- Console_findCmd: can't find this cmd!\n");
    return 0;
}

/**
 * @brief 显示命令输入提示符。
 * @details 提示符定义为： @ref CONSOLE_PROMPT_STRING 。
 */
void Console_Prompt(void)
{
    Console_Out(CONSOLE_PROMPT_STRING);
}

/**
 * @brief 控制台格式化输出函数。
 * @param strFormat 格式化字符串。
 * @details 类似printf的另一个简单实现。
 * @note 本格式化输出函数暂时仅支持以下格式化控制符：
 *  - %c ：输出一个字符
 *  - %s ：输出一个字符串
 *  - %d ：输出一个整数
 *  - %u ：输出一个无符号整数
 *  - %p ：输出一个地址（十六进制）
 *  - %x 和 %X ：以十六进制格式输出一个整数
 */
void Console_Out(const char *strFormat, ...)
{
    va_list args;
    uint8_t Length;

    /* Write out the Data.                                         */
    va_start(args, strFormat);
    Length = vsnprintf((char*)s_bufTx, (sizeof(s_bufTx) - 1), strFormat, args);
    va_end(args);
    StrLine_WriteData((uint8_t*)s_bufTx, Length);
}

void Console_HexGrounpOut(uint8_t* hexGroup, uint8_t len, char separator)
{
    if(3*len+3+3 > CONSOLE_TX_BUF_SIZE)
    {   //长度超过限制，只打印前面一部分内容
        len = CONSOLE_TX_BUF_SIZE/3 -1 -1;
    }
    s_bufTx[0] = '[';
    HexGroupToString(hexGroup,len,(uint8_t*)s_bufTx+1, separator);

    if(len == CONSOLE_TX_BUF_SIZE/3 -1 -1)
    {
        memcpy(s_bufTx+3*len+1, "...",3 );
        s_bufTx[3*len+3+1] = ']';
        s_bufTx[3*len+3+2] = '\n';
        StrLine_WriteData(s_bufTx, 3*len+3+3);
    }
    else
    {
        s_bufTx[3*len+1] = ']';
        s_bufTx[3*len+2] = '\n';
        StrLine_WriteData(s_bufTx, 3*len+3);
    }
}

/**
 * @brief 输出一个字符串到控制台。
 * @param str 要输出的字符串。
 */
void Console_OutData(uint8_t * str, uint8_t len)
{
    StrLine_WriteData((uint8_t *)str, len);
}


/**
 * @brief 显示欢迎信息。
 */
void Console_Welcome(void)
{
#if (CONSOLE_NEED_WELCOME_MSG == 1)

    // 打印欢迎信息
    Console_Out("\n\n----------------\n");
    Console_Out("Welcome to use %s Console\n", CONSOLE_WELCOME_PRODUCT);

    // 打印版本号
#if (CONSOLE_WELCOME_NEED_VERSION == 1)
    Console_Out("ConsoleVersion: %u.%u.%u.%u\n",
            g_kCmdLineVersion.major,
            g_kCmdLineVersion.minor,
            g_kCmdLineVersion.revision,
            g_kCmdLineVersion.build
            );
#endif // CONSOLE_WELCOME_NEED_VERSION

#ifdef CONSOLE_WELCOME_COPYRIGHT
    Console_Out(CONSOLE_WELCOME_COPYRIGHT);
#endif // CONSOLE_WELCOME_COPYRIGHT

#ifdef CONSOLE_WELCOME_OTHER
    Console_Out("\n%s\n\n", CONSOLE_WELCOME_OTHER);
#endif // CONSOLE_WELCOME_OTHER
    Cmd_TraceGetLevel(0,NULL);
    Console_Out("Board message is:%s.\n", BOARD_MESG);
    Console_Out("Compilie version is:-----------------------------%s.\n", COMPILIE_VERSION);
    ManufVersion softVersion = VersionInfo_GetSoftwareVersion();
    Console_Out("\nBoard SoftVersion: %u.%u.%u.%u\n",
            softVersion.major,
            softVersion.minor,
            softVersion.revision,
            softVersion.build
            );
    Console_Out("\n\n");
//    Console_Prompt();

#endif // CONSOLE_NEED_WELCOME_MSG
}
