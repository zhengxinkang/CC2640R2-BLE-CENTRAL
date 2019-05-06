/*
 * IWUP_frame.c
 *
 *  Created on: 2017��12��19��
 *      Author: Administrator
 */
#include "IwupUart_adapter.h"
#include "IWUP_frame.h"
#include "IWUP_config.h"
#include "IWUP_cmd.h"
#include "IWUP_notify.h"
#include "Trace.h"

uint8_t frameRx[FRAME_MAX_SIZE];
uint8_t frameTx[FRAME_MAX_SIZE];
uint8_t frameTxEsc[FRAME_MAX_SIZE+10];//ת�������ݣ�2���ֽ�֡ͷ֡β���������ת������8��
static IwupFrame* iwupRxFrame = (IwupFrame*) frameRx;
static IwupFrame* iwupTxFrame = (IwupFrame*) frameTx;
static uint8_t frameSerial = 0;

static uint16_t IwupUnpack_Sync(uint8_t* data, uint16_t len);
static uint8_t IwupPack_Sync(uint8_t* data, uint16_t len);
static uint8_t checkByte(uint8_t *data, uint8_t len);

void IWUP_frame_send(FRAME_TYPE type, uint8_t cmdGroup, uint8_t cmdCode, uint8_t* para, uint8_t paraLen, uint8_t portNum)
{
//para
    if (FRAME_MAX_SIZE >= paraLen)
    {
        memcpy(iwupTxFrame->para, para, paraLen);
    }
    else
    {
        TRACE_CODE("IWUP_frame_send: ERRPR! para too long!max=[%d],now=[%d]\n", COMMAND_PARA_LEN_MAX, paraLen);
        return;
    }
//paraLen
    iwupTxFrame->paraLen = paraLen;
//cmdCode
    iwupTxFrame->cmdCode = cmdCode;
//cmdGroup
    iwupTxFrame->cmdGroup = cmdGroup;
//frameSerial
    iwupTxFrame->frameSerial = frameSerial;
//CtrWord
#if defined     IWUP_ROLE_WIRELESS_MODULE
    iwupTxFrame->ctrlWord.dir = FRAME_LL_DIR_DOWN;
#elif defined   IWUP_ROLE_LOCK_BOARD
    iwupTxFrame->ctrlWord.dir = FRAME_LL_DIR_UP;
#else
    #error "role NOT DEFINE!"
#endif
    iwupTxFrame->ctrlWord.frameType = type;
    iwupTxFrame->ctrlWord.reserved = 0;
//CheckData
    iwupTxFrame->checkData = checkByte((uint8_t*)&(iwupTxFrame->ctrlWord), iwupTxFrame->paraLen +FRAME_HEADER -1);
//ת��,��֡ͷ֡β
    uint8_t lenValid = paraLen + FRAME_HEADER;
    uint8_t lenSend = IwupPack_Sync(frameTx, lenValid);
    Uart_adapter_send(frameTxEsc, lenSend, portNum);
}

typedef enum
{
    RET_IWUP_FRAME_RECIEVE_OK,
    RET_IWUP_FRAME_RECIEVE_ERROR_ESC,   //ת�����
    RET_IWUP_FRAME_RECIEVE_ERROR_LEN,   //���ȴ���
    RET_IWUP_FRAME_RECIEVE_ERROR_CHECK, //У�����
    RET_IWUP_FRAME_RECIEVE_ERROR_DIR,   //�������
    RET_IWUP_FRAME_RECIEVE_ERROR_TYPE,  //֡���ʹ���
    RET_IWUP_FRAME_RECIEVE_ERROR_PARALEN,//�������ȴ���
    RET_IWUP_FRAME_RECIEVE_ERROR_TYPE_NOFIND,//δ�ҵ�������
} RET_IWUP_FRAME_RECIEVE;

void IWUP_frame_recieve(uint8_t* data, uint16_t len)
{
    RET_IWUP_FRAME_RECIEVE ret = RET_IWUP_FRAME_RECIEVE_OK;

/*֡ͷ��֡β�жϣ� ��ת��*/
    uint16_t frameValidLen;
    if(RET_IWUP_FRAME_RECIEVE_OK == ret)
    {
        frameValidLen = IwupUnpack_Sync(data, len);
        if(0 == frameValidLen)
        {
            TRACE_CODE("IWUP_frame_recieve: recieve error!\n");
            ret = RET_IWUP_FRAME_RECIEVE_ERROR_ESC;
        }
        else if(FRAME_MIN_SIZE > frameValidLen)
        {
            TRACE_CODE("IWUP_frame_recieve: frameValidLen too short,%d!\n", frameValidLen);
            ret = RET_IWUP_FRAME_RECIEVE_ERROR_LEN;
        }
        else
        {
            TRACE_MARK("IWUP_frame_recieve: frame header tail ok,esc ok.\n");
            ret = RET_IWUP_FRAME_RECIEVE_OK;
        }
    }
/*У���*/
    if(RET_IWUP_FRAME_RECIEVE_OK == ret)
    {
        uint8_t checkResult = checkByte((uint8_t*)&(iwupRxFrame->ctrlWord), iwupRxFrame->paraLen +FRAME_HEADER -1);
        if(checkResult == iwupRxFrame->checkData)
        {
            TRACE_MARK("IWUP_frame_recieve: check ok.\n");
            ret = RET_IWUP_FRAME_RECIEVE_OK;
        }
        else
        {
            ret = RET_IWUP_FRAME_RECIEVE_ERROR_CHECK;
            TRACE_CODE("IWUP_frame_recieve:RET_IWUP_FRAME_RECIEVE_ERROR_CHECK!\n");
        }
    }

/*��·�����ж�-���ݷ����ж�*/
    if(RET_IWUP_FRAME_RECIEVE_OK == ret)
    {
#if defined     IWUP_ROLE_WIRELESS_MODULE
        if(FRAME_LL_DIR_UP == iwupRxFrame->ctrlWord.dir)
#elif defined   IWUP_ROLE_LOCK_BOARD
        if(FRAME_LL_DIR_DOWN == iwupRxFrame->ctrlWord.dir)
#else
    #error "role NOT DEFINE!"
#endif
        {
            ret = RET_IWUP_FRAME_RECIEVE_OK;
            TRACE_MARK("IWUP_frame_recieve: direction ok.\n");
        }
        else
        {
            ret = RET_IWUP_FRAME_RECIEVE_ERROR_DIR;
            TRACE_CODE("IWUP_frame_recieve:RET_IWUP_FRAME_RECIEVE_ERROR_DIR!\n");
        }
    }

/*��·�����ж�-֡�����ж�*/
    if (RET_IWUP_FRAME_RECIEVE_OK == ret)
    {
#if defined     IWUP_ROLE_WIRELESS_MODULE
        if(FRAME_TYPE_COMMAND_RESP == iwupRxFrame->ctrlWord.frameType||FRAME_TYPE_NOTIFY == iwupRxFrame->ctrlWord.frameType)
#elif defined   IWUP_ROLE_LOCK_BOARD
        if(FRAME_TYPE_COMMAND == iwupRxFrame->ctrlWord.frameType||FRAME_TYPE_NOTIFY_RESP == iwupRxFrame->ctrlWord.frameType)
#else
    #error "role NOT DEFINE!"
#endif
        {
            TRACE_MARK("IWUP_frame_recieve: frame type ok.\n");
            ret = RET_IWUP_FRAME_RECIEVE_OK;
        }
        else
        {
            ret = RET_IWUP_FRAME_RECIEVE_ERROR_TYPE;
            TRACE_CODE("IWUP_frame_recieve:RET_IWUP_FRAME_RECIEVE_ERROR_TYPE!\n");
        }
    }
/*���������ж�*/
    if (RET_IWUP_FRAME_RECIEVE_OK == ret)
    {
        uint8_t paraLenCount = frameValidLen - FRAME_HEADER;
        if(paraLenCount == iwupRxFrame->paraLen)
        {
            TRACE_MARK("IWUP_frame_recieve: para len check ok.\n");
            ret = RET_IWUP_FRAME_RECIEVE_OK;
        }
        else
        {
            TRACE_CODE("IWUP_frame_recieve:RET_IWUP_FRAME_RECIEVE_ERROR_PARALEN!\n");
            ret = RET_IWUP_FRAME_RECIEVE_ERROR_PARALEN;
        }
    }
/*�����֪ͨ·��*/
    if(RET_IWUP_FRAME_RECIEVE_OK == ret)
    {
        TRACE_MARK("IWUP_cmd_recieve:");
        TRACE_MARK_HEXGROUP(frameRx, frameValidLen, ' ');
        TRACE_MARK("IWUP_frame_recieve: recieve %d bytes.\n", frameValidLen);

        switch(iwupRxFrame->ctrlWord.frameType)
        {
#if defined     IWUP_ROLE_WIRELESS_MODULE
            case FRAME_TYPE_COMMAND_RESP:
            {
                Recieve_commandResp(iwupRxFrame->cmdGroup, iwupRxFrame->cmdCode, iwupRxFrame->para, iwupRxFrame->paraLen);
            }
            break;

            case FRAME_TYPE_NOTIFY:
            {
                Recieve_notify(iwupRxFrame->cmdGroup, iwupRxFrame->cmdCode, iwupRxFrame->para, iwupRxFrame->paraLen);
            }
            break;
#elif defined   IWUP_ROLE_LOCK_BOARD
            case FRAME_TYPE_COMMAND:
            {
                Cmd_recieve(iwupRxFrame->cmdGroup, iwupRxFrame->cmdCode, iwupRxFrame->para, iwupRxFrame->paraLen);
            }
            break;

            case FRAME_TYPE_NOTIFY_RESP:
            {
                Recieve_notifyResp(iwupRxFrame->cmdGroup, iwupRxFrame->cmdCode, iwupRxFrame->para, iwupRxFrame->paraLen);
            }
            break;
#else
    #error "role no define!"
#endif
            default:
            {   //should not be here.
                TRACE_CODE("IWUP_frame_recieve: frame type not find!\n");
                ret = RET_IWUP_FRAME_RECIEVE_ERROR_TYPE_NOFIND;
            }

        }
    }
/*����λ*/
    if(RET_IWUP_FRAME_RECIEVE_OK == ret)
    {
        frameSerial = iwupRxFrame->frameSerial;
    }
    else
    {
        TRACE_CODE("IWUP_frame_recieve: ERROR! error code=[%d]\n", ret);
    }
}

/* IwupPack_Sync
 * ���ƣ�֡ͬ�����
 * ���ã�����ת�塢���֡ͷ֡β
 * */
static uint8_t IwupPack_Sync(uint8_t* data, uint16_t len)
{
    uint8_t lenAfterEsc = 0;
    frameTxEsc[lenAfterEsc++] = FRAME_CHAR_SYNC;
    for(uint16_t i=0; i<len; i++)
    {
        if(FRAME_CHAR_SYNC == data[i])
        {
            frameTxEsc[lenAfterEsc++] = FRAME_CHAR_ESC;
            frameTxEsc[lenAfterEsc++] = FRAME_CHAR_RAW_SYNC;
        }
        else if(FRAME_CHAR_ESC == data[i])
        {
            frameTxEsc[lenAfterEsc++] = FRAME_CHAR_ESC;
            frameTxEsc[lenAfterEsc++] = FRAME_CHAR_RAW_ESC;
        }
        else
        {
            frameTxEsc[lenAfterEsc++] = data[i];
        }
    }
    frameTxEsc[lenAfterEsc++] = FRAME_CHAR_SYNC;
    return lenAfterEsc;
}

/* IwupUnpack_Sync
 * ���ƣ�֡ͬ�����
 * ���ã�֡ͷ֡β�жϡ���ת��
 * ע�⣺�����֡����һ��������������һ֡����
 * */
typedef enum
{
    RET_IWUPUNPACK_SYNC_ERROR,
    RET_IWUPUNPACK_SYNC_FINISH,
    RET_IWUPUNPACK_SYNC_ESC_ERROR,
}RET_IWUPUNPACK_SYNC;

static uint16_t IwupUnpack_Sync(uint8_t* data, uint16_t len)
{
    RET_IWUPUNPACK_SYNC ret = RET_IWUPUNPACK_SYNC_ERROR;
    uint16_t frameGetLen = 0;

    for(uint16_t i=0; i<len; i++)
    {
        if(FRAME_CHAR_SYNC == data[i])      //7E
        {
            if(frameGetLen > 0) //֡β��
            {
//                if(0x00 == checkByte(frameRx ,frameGetLen)&&0x00 == frameRx[0])//����������֡����У���Ҳ������0
                if(0)
                {//һ֡0x00���ݣ�����֮
                    frameGetLen = 0;
                }
                else
                {//һ֡��Ч����
                     ret = RET_IWUPUNPACK_SYNC_FINISH;
                     break;
                }
            }
            else                //֡ͷ����������ֱ������
            {
            }
        }
        else if(FRAME_CHAR_ESC == data[i])  //7D
        {
            if (FRAME_CHAR_RAW_SYNC == data[i + 1])
              {
                  frameRx[frameGetLen++] = FRAME_CHAR_SYNC;
                  i++; //��������Ҫ��ת�����ݣ�5D��5E��
              }
              else if (FRAME_CHAR_RAW_ESC == data[i + 1])
              {
                  frameRx[frameGetLen++] = FRAME_CHAR_ESC;
                  i++; //��������Ҫ��ת�����ݣ�5D��5E��
              }
              else
              {   //���󣬲����ڵ�ת��
                  ret = RET_IWUPUNPACK_SYNC_ESC_ERROR;
                  TRACE_CODE("IwupUnpack_Sync:RET_IWUPUNPACK_SYNC_ESC_ERROR!");
                  break;
              }
        }
        else                                //��ͨ����
        {
            frameRx[frameGetLen++] = data[i];
        }
    }
    /* ��������
     * 1�������ɹ�
     * 2��ת�����ת���7D������Ĳ���5D��5E
     * 3��δ������ͬ����
     * */
    if(RET_IWUPUNPACK_SYNC_FINISH == ret)
    {
        return frameGetLen;
    }
    else
    {
        return 0;
    }
}

static uint8_t checkByte(uint8_t *data, uint8_t len)
{
    uint32_t sum = 0;
    uint8_t ret_checkData = 0;
    for(uint8_t i=0; i<len; i++)
    {
        sum += *(data+i);
    }
    ret_checkData = sum&0xff;
    return ret_checkData;
}
