/*
 * Console.h
 *
 *  Created on: 2017��8��9��
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_CONSOLE_CONSOLE_H_
#define SOURCE_CONSOLE_CONSOLE_H_


#include <stdint.h>
#include "ConsoleConfig.h"
// ��ʼ��
extern void Console_Init(void);

// ��̨����
extern int Console_RoutineHandle(void);

// �����Ϣ������̨
extern void Console_Prompt(void);
extern void Console_Welcome(void);

// ��ʽ����������ӿ�
extern void Console_Out(const char * strFormat, ...);

//�������ַ�����ʽ���16��������
extern void Console_HexGrounpOut(uint8_t* hexGroup, uint8_t len, char separator);

// ����ֱ�����
//extern void Console_OutString(const char * str);
extern void Console_OutData(uint8_t* str, uint8_t len);
extern uint8_t s_bufTx[CONSOLE_TX_BUF_SIZE];
//*******************************************************************
// ��������ӿں�

/**
 * @brief һ����ʵ�ֵĸ�ʽ�����������
 * @details �����Ǻ��� @ref Console_Out() �ĺ궨�塣
 */
#define ConsoleOut          Console_Out

/**
 * @brief һ����ʵ�ֵĸ�ʽ�����������
 * @details �����Ǻ��� Console_Out() �ĺ궨�塣
 */
#define EM_TRACE            Console_Out


//*******************************************************************
// ����������������

/**
 * @brief �ӿں���ԭ�Ͷ��壺����̨���������
 * @param argc ����������
 * @param argv ��������б�
 *  argv[0]Ϊ����ؼ��ֱ���argv[1]��ʼ������Ĳ����ִ���
 * @return ����ִ��״̬��0 ��ʾ�ɹ�ִ�У��� 0 Ϊ�쳣���μ� @ref ConsoleCmdRetCode
 */
typedef int (*IfConsoleCommandHandle)(int argc, char *argv[]);


/**
 * @brief �������������͡�
 */
typedef struct
{
    const char * cmdKeyword;            ///< ����ؼ��֣��ַ�����
    IfConsoleCommandHandle cmdHandle;   ///< �������
    const char * cmdHelp;               ///< ����˵���ı�
}
CmdLineEntry;


/**
 * @brief �������������ؼ������䴦����֮��Ķ�Ӧ��ϵ��
 * @details ������������û��ӿ�ģ�� @ref CmdLine.c �ж��塣
 * @note Ӧ�ó�������ṩ�ñ�
 */
extern const CmdLineEntry g_kConsoleCmdTable[];


//*******************************************************************
// �����а汾����

/**
 * @brief �����а汾��
 */
typedef struct
{
    unsigned short major;       ///< ���汾��
    unsigned short minor;       ///< �ΰ汾��
    unsigned short revision;    ///< �޶��汾��
    unsigned short build;       ///< ����汾��
}
CmdLineVersion;

/**
 * @brief �����а汾�š�
 * @note Ӧ�ó��� @ref CmdLine.c �����붨��ð汾�ţ����Ҹ�����Ҫ���¡�
 */
extern const CmdLineVersion g_kCmdLineVersion;


//*******************************************************************
// ���ؽ���붨��

/**
 * @brief ���ؽ���붨��
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
