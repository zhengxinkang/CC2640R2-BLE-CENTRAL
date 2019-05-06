/*
 * IWUP_cmd.h
 *
 *  Created on: 2017年12月19日
 *      Author: Administrator
 */

#ifndef SOURCE_PROTOCOL_IWUP_IWUP_CMD_H_
#define SOURCE_PROTOCOL_IWUP_IWUP_CMD_H_


#include "IWUP_config.h"

typedef uint16_t CmdGroupCode;         ///< 命令码类型。

typedef void (*IfIwupCmdHandle)(uint8_t* para, uint8_t paraLen);
/**
 * @brief DSCP 命令表结点
 */
typedef struct IwupCmdEntryStruct
{
    CmdGroupCode cmdGroupCode;                ///< 命令码
    IfIwupCmdHandle handle;         ///< 命令处理函数
}
IwupCmdEntry;

#define IWUP_CMD_ENTRY(CmdGroupCode, handle)    {CmdGroupCode, (IfIwupCmdHandle)handle}// 定义一个命令结点，指定命令码对应的处理函数。
#define IWUP_CMD_ENTRY_END          {0, 0} //定义一个命令表的结束结点。

typedef IwupCmdEntry* IwupCmdTable;    ///< 命令表类型。


#if defined     IWUP_ROLE_WIRELESS_MODULE

void Recieve_commandResp    (uint8_t cmdGroup, uint8_t cmdCode, uint8_t* para, uint8_t paraLen);
void Cmd_send               (uint16_t cmdGroupCode, uint8_t* para, uint8_t paraLen, uint8_t portNum);

#elif defined   IWUP_ROLE_LOCK_BOARD

void Cmd_recieve            (uint8_t cmdGroup, uint8_t cmdCode, uint8_t* para, uint8_t paraLen);
void CmdResp_send           (uint16_t cmdGroupCode, uint8_t* para, uint8_t paraLen, uint8_t portNum);

#else
    #error "role NOT DEFINE!"
#endif


#endif /* SOURCE_PROTOCOL_IWUP_IWUP_CMD_H_ */
