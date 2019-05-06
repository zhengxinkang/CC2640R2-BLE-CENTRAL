/*
 * Console.c
 *
 *  Created on: 2017��8��9��
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
// �����н���״̬
#define CONSOLE_EVENT_NONE              0   // ��������
#define CONSOLE_EVENT_NEW_CMD           1   // ������
#define CONSOLE_EVENT_TERMINATE         2   // ��ֹ����������е�ǰ����
static volatile unsigned char s_consoleEvent = CONSOLE_EVENT_NONE;

// �����н������
#define CMDLINE_BAD_CMD         (-1)    // ϵͳ��֧�ֵ����δ���壩
#define CMDLINE_TOO_MANY_ARGS   (-2)    // ����������в���̫��
#define CMDLINE_NO_CMD          (-3)    // ������

//************************************
// �����н��ջ����д����
uint8_t s_bufTx[CONSOLE_TX_BUF_SIZE];
static void Console_scanf(uint8_t *string, uint16_t len);

/**
 * @brief ����̨��ʼ����
 */
void Console_Init(void)
{
    StrLine_init();
    StrLine_recieveToUp_setFunc(Console_scanf);
    Console_Welcome();
}

/**
 * @brief �����������ı���ִ�����
 * @param cmdLine �������ı��ַ�����
 * @return ������ִ��״̬��
 */
int Console_findCmd(int argc, char *argv[]);
//�����ַ���-ת��Ϊ����
void Console_scanf(uint8_t *string, uint16_t len)
{
    static char *argv[CONSOLE_MAX_ARGUMENTS + 1];
    char * pos;
    int argc;
    int bFindArg = 1;
    uint8_t char_index=0;

    // ��ʼ��������Ϊ�գ�����Ӧ���������
    for (argc = 0; argc <= CONSOLE_MAX_ARGUMENTS; argc++)
        argv[argc] = 0;

    // ��ʼ����������
    argc = 0;
    pos = (char*)string;

    // ���������ı�ֱ���������ַ�����ʾ���ı�����
    for(char_index=0; char_index<len; char_index++)
    {
        // �հ��ַ���һ�������� ��ǰ������
        if(*pos == ' ' || *pos == '\t')
        {
            *pos = 0;
            bFindArg = 1;
        }
        else
        {
            // �зǿհ��ַ���һ��������ʼ
            // ��һ�����ҵ�������
            if(bFindArg)
            {
                if(argc < CONSOLE_MAX_ARGUMENTS)
                {
                    // ��¼�ò���
                    argv[argc] = pos;
                    argc++;
                    bFindArg = 0;
                }
                else
                {
                    // �������ֹ࣬ͣ����
                    return;
                }
            }
        }

        pos++;
    }

    // �н�������������һ��������������ؼ���
    if(argc)
    {
        Console_findCmd(argc, argv);
    }
}

/*****************************************
���ݲ���������Ѱ�Ҷ�Ӧ������
******************************************/
int Console_findCmd(int argc, char *argv[])
{
    CmdLineEntry *cmdEntry;
    // �������������ƥ��ؼ���
    cmdEntry = (CmdLineEntry *) &g_kConsoleCmdTable[0];
    TRACE_CODE("CONSOLE INPUT CMD: ");
    Cmd_showparam(argc, argv);
    while(cmdEntry->cmdKeyword)
    {
        // ƥ������ؼ��֣�������Ӧ���������
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
 * @brief ��ʾ����������ʾ����
 * @details ��ʾ������Ϊ�� @ref CONSOLE_PROMPT_STRING ��
 */
void Console_Prompt(void)
{
    Console_Out(CONSOLE_PROMPT_STRING);
}

/**
 * @brief ����̨��ʽ�����������
 * @param strFormat ��ʽ���ַ�����
 * @details ����printf����һ����ʵ�֡�
 * @note ����ʽ�����������ʱ��֧�����¸�ʽ�����Ʒ���
 *  - %c �����һ���ַ�
 *  - %s �����һ���ַ���
 *  - %d �����һ������
 *  - %u �����һ���޷�������
 *  - %p �����һ����ַ��ʮ�����ƣ�
 *  - %x �� %X ����ʮ�����Ƹ�ʽ���һ������
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
    {   //���ȳ������ƣ�ֻ��ӡǰ��һ��������
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
 * @brief ���һ���ַ���������̨��
 * @param str Ҫ������ַ�����
 */
void Console_OutData(uint8_t * str, uint8_t len)
{
    StrLine_WriteData((uint8_t *)str, len);
}


/**
 * @brief ��ʾ��ӭ��Ϣ��
 */
void Console_Welcome(void)
{
#if (CONSOLE_NEED_WELCOME_MSG == 1)

    // ��ӡ��ӭ��Ϣ
    Console_Out("\n\n----------------\n");
    Console_Out("Welcome to use %s Console\n", CONSOLE_WELCOME_PRODUCT);

    // ��ӡ�汾��
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
