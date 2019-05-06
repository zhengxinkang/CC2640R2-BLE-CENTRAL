/*
 * IWUP_cmd.c
 *
 *  Created on: 2017年12月19日
 *      Author: Administrator
 */
#include <IWUP_cmd_group/CmdGroup_qcTest.h>
#include "IWUP_cmd.h"
#include "IWUP_config.h"
#include "IWUP_frame.h"

#if defined     IWUP_ROLE_WIRELESS_MODULE

static const IwupCmdEntry s_lIwupCmdRespTable[] =
{
    TABLE_RECIEVE_CMDGROUP_RESP_QCTEST,
    IWUP_CMD_ENTRY_END
};

static IwupCmdTable CmdRespTable_GetTable(void)
{
    return (IwupCmdTable) s_lIwupCmdRespTable;
}


void Recieve_commandResp(uint8_t cmdGroup, uint8_t cmdCode, uint8_t* para, uint8_t paraLen)
{
    CmdGroupCode cmdGroupCode = 0x0000;
    cmdGroupCode = (cmdGroup<<8) + cmdCode;

    IwupCmdEntry* cmdEntry;

    cmdEntry = CmdRespTable_GetTable();
    while (cmdEntry->handle)
    {
        if (cmdEntry->cmdGroupCode == cmdGroupCode)
        {   // 调用命令处理函数
            cmdEntry->handle(para, paraLen);
            break;
        }

        cmdEntry++;
        if (NULL == cmdEntry->handle)
        {
            // @todo 未在命令表中找到命令，需要特殊处理
            TRACE_ERROR("Recieve_commandResp: can't find this command,code=[%04x]!\n",cmdGroupCode);
            break;
        }
    }
}

void Cmd_send         (uint16_t cmdGroupCode, uint8_t* para, uint8_t paraLen, uint8_t portNum)
{
    IWUP_frame_send(FRAME_TYPE_COMMAND, cmdGroupCode>>8, cmdGroupCode&0x00ff, para, paraLen, portNum);
}

#elif defined   IWUP_ROLE_LOCK_BOARD

//---------------
static const IwupCmdEntry s_lIwupCmdTable[] =
{
    TABLE_RECIEVE_CMDGROUP_GENERAL,
    TABLE_RECIEVE_CMDGROUP_KEY_MANAGE,
    TABLE_RECIEVE_CMDGROUP_ACTION,
    TABLE_RECIEVE_CMDGROUP_QCTEST,
    IWUP_CMD_ENTRY_END
};

static IwupCmdTable CmdTable_GetTable(void)
{
    return (IwupCmdTable) s_lIwupCmdTable;
}

void Cmd_recieve(uint8_t cmdGroup, uint8_t cmdCode, uint8_t* para, uint8_t paraLen)
{
    CmdGroupCode cmdGroupCode = 0x0000;
    cmdGroupCode = (cmdGroup<<8) + cmdCode;

    IwupCmdEntry* cmdEntry;

    cmdEntry = CmdTable_GetTable();
    while (cmdEntry->handle)
    {
        if (cmdEntry->cmdGroupCode == cmdGroupCode)
        {   // 调用命令处理函数
            cmdEntry->handle(para, paraLen);
            break;
        }

        cmdEntry++;
        if (NULL == cmdEntry->handle)
        {
            // @todo 未在命令表中找到命令，需要特殊处理
            TRACE_ERROR("Recieve_command: can't find this command,code=[%04x]!\n",cmdGroupCode);
            break;
        }
    }
}

void CmdResp_send         (uint16_t cmdGroupCode, uint8_t* para, uint8_t paraLen, uint8_t portNum)
{
    IWUP_frame_send(FRAME_TYPE_COMMAND_RESP, cmdGroupCode>>8, cmdGroupCode&0x00ff, para, paraLen, portNum);
}

#else
    #error "role NOT DEFINE!"
#endif
