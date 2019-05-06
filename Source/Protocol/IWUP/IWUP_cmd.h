/*
 * IWUP_cmd.h
 *
 *  Created on: 2017��12��19��
 *      Author: Administrator
 */

#ifndef SOURCE_PROTOCOL_IWUP_IWUP_CMD_H_
#define SOURCE_PROTOCOL_IWUP_IWUP_CMD_H_


#include "IWUP_config.h"

typedef uint16_t CmdGroupCode;         ///< ���������͡�

typedef void (*IfIwupCmdHandle)(uint8_t* para, uint8_t paraLen);
/**
 * @brief DSCP �������
 */
typedef struct IwupCmdEntryStruct
{
    CmdGroupCode cmdGroupCode;                ///< ������
    IfIwupCmdHandle handle;         ///< �������
}
IwupCmdEntry;

#define IWUP_CMD_ENTRY(CmdGroupCode, handle)    {CmdGroupCode, (IfIwupCmdHandle)handle}// ����һ�������㣬ָ���������Ӧ�Ĵ�������
#define IWUP_CMD_ENTRY_END          {0, 0} //����һ�������Ľ�����㡣

typedef IwupCmdEntry* IwupCmdTable;    ///< ��������͡�


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
