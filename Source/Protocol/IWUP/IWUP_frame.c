/*
 * IWUP_frame.c
 *
 *  Created on: 2017年12月19日
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
uint8_t frameTxEsc[FRAME_MAX_SIZE+10];//转义后的数据，2个字节帧头帧尾，理论最大转义数据8个
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
//转义,加帧头帧尾
    uint8_t lenValid = paraLen + FRAME_HEADER;
    uint8_t lenSend = IwupPack_Sync(frameTx, lenValid);
    Uart_adapter_send(frameTxEsc, lenSend, portNum);
}

typedef enum
{
    RET_IWUP_FRAME_RECIEVE_OK,
    RET_IWUP_FRAME_RECIEVE_ERROR_ESC,   //转义错误
    RET_IWUP_FRAME_RECIEVE_ERROR_LEN,   //长度错误
    RET_IWUP_FRAME_RECIEVE_ERROR_CHECK, //校验错误
    RET_IWUP_FRAME_RECIEVE_ERROR_DIR,   //方向错误
    RET_IWUP_FRAME_RECIEVE_ERROR_TYPE,  //帧类型错误
    RET_IWUP_FRAME_RECIEVE_ERROR_PARALEN,//参数长度错误
    RET_IWUP_FRAME_RECIEVE_ERROR_TYPE_NOFIND,//未找到该类型
} RET_IWUP_FRAME_RECIEVE;

void IWUP_frame_recieve(uint8_t* data, uint16_t len)
{
    RET_IWUP_FRAME_RECIEVE ret = RET_IWUP_FRAME_RECIEVE_OK;

/*帧头，帧尾判断； 反转义*/
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
/*校验和*/
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

/*链路控制判断-数据方向判断*/
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

/*链路控制判断-帧类型判断*/
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
/*参数长度判断*/
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
/*命令或通知路由*/
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
/*错误定位*/
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
 * 名称：帧同步打包
 * 作用：进行转义、添加帧头帧尾
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
 * 名称：帧同步解包
 * 作用：帧头帧尾判断、反转义
 * 注意：如果两帧数据一起分析则丢弃后面的一帧数据
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
            if(frameGetLen > 0) //帧尾：
            {
//                if(0x00 == checkByte(frameRx ,frameGetLen)&&0x00 == frameRx[0])//致命错误，整帧数据校验和也可能是0
                if(0)
                {//一帧0x00数据，忽略之
                    frameGetLen = 0;
                }
                else
                {//一帧有效数据
                     ret = RET_IWUPUNPACK_SYNC_FINISH;
                     break;
                }
            }
            else                //帧头：不做处理直接跳过
            {
            }
        }
        else if(FRAME_CHAR_ESC == data[i])  //7D
        {
            if (FRAME_CHAR_RAW_SYNC == data[i + 1])
              {
                  frameRx[frameGetLen++] = FRAME_CHAR_SYNC;
                  i++; //跳过不需要的转义数据（5D或5E）
              }
              else if (FRAME_CHAR_RAW_ESC == data[i + 1])
              {
                  frameRx[frameGetLen++] = FRAME_CHAR_ESC;
                  i++; //跳过不需要的转义数据（5D或5E）
              }
              else
              {   //错误，不存在的转义
                  ret = RET_IWUPUNPACK_SYNC_ESC_ERROR;
                  TRACE_CODE("IwupUnpack_Sync:RET_IWUPUNPACK_SYNC_ESC_ERROR!");
                  break;
              }
        }
        else                                //普通数据
        {
            frameRx[frameGetLen++] = data[i];
        }
    }
    /* 解析结束
     * 1、解析成功
     * 2、转义错误，转义符7D后面跟的不是5D或5E
     * 3、未解析到同步符
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
