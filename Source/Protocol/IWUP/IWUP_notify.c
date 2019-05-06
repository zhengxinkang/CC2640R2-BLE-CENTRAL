/*
 * IWUP_notify.c
 *
 *  Created on: 2017年12月19日
 *      Author: Administrator
 */
#include <IWUP_notify_group/NotifyGroup_qcTest.h>
#include "IWUP_cmd.h"
#include "IWUP_frame.h"
#include "IWUP_config.h"


#if defined     IWUP_ROLE_WIRELESS_MODULE
static const IwupCmdEntry s_lIwupNotifyTable[] =
{
    TABLE_RECIEVE_NOTIFYGROUP_QCTEST,
    IWUP_CMD_ENTRY_END
};

static IwupCmdTable NotifyTable_GetTable(void)
{
    return (IwupCmdTable) s_lIwupNotifyTable;
}

void Recieve_notify     (uint8_t cmdGroup, uint8_t cmdCode, uint8_t* para, uint8_t paraLen)
{
    CmdGroupCode cmdGroupCode = 0x0000;
    cmdGroupCode = (cmdGroup<<8) + cmdCode;

    IwupCmdEntry* cmdEntry;

    cmdEntry = NotifyTable_GetTable();
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
            TRACE_ERROR("Recieve_notify: can't find this command,code=[%04x]\n",cmdGroupCode);
            break;
        }
    }
}

void NotifyResp         (uint16_t cmdGroupCode, uint8_t* para, uint8_t paraLen, uint8_t portNum)
{
    IWUP_frame_send(FRAME_TYPE_NOTIFY_RESP, cmdGroupCode>>8, cmdGroupCode&0x00ff, para, paraLen, portNum);
}

#elif defined   IWUP_ROLE_LOCK_BOARD

static const IwupCmdEntry s_lIwupNotifyRespTable[] =
{
    TABLE_RECIEVE_NOTIFYGROUP_RESP_GENERAL,
    TABLE_RECIEVE_NOTIFYGROUP_RESP_QCTEST,
    IWUP_CMD_ENTRY_END
};

static IwupCmdTable NotifyRespTable_GetTable(void)
{
    return (IwupCmdTable) s_lIwupNotifyRespTable;
}

void Recieve_notifyResp(uint8_t cmdGroup, uint8_t cmdCode, uint8_t* para, uint8_t paraLen)
{
    CmdGroupCode cmdGroupCode = 0x0000;
    cmdGroupCode = (cmdGroup<<8) + cmdCode;

    IwupCmdEntry* cmdEntry;

    cmdEntry = NotifyRespTable_GetTable();
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
            TRACE_ERROR("Recieve_notifyResp: can't find this command,code=[%04x]\n",cmdGroupCode);
            break;
        }
    }
}

void Notify_send         (uint16_t cmdGroupCode, uint8_t* para, uint8_t paraLen, uint8_t portNum)
{
    IWUP_frame_send(FRAME_TYPE_NOTIFY, cmdGroupCode>>8, cmdGroupCode&0x00ff, para, paraLen, portNum);
}

#else
#error "role NOT DEFINE!"
#endif
