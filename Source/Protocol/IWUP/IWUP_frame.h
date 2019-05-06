/*
 * IWUP_frame.h
 *
 *  Created on: 2017年12月19日
 *      Author: Administrator
 */

#ifndef _IWUP_FRAME_H_
#define _IWUP_FRAME_H_


#include "IWUP_config.h"
/*********************************************************************
 * MACROS
 */
/*
frame type
LEN:        |   1       |   1       |   1       |   1           |   1       |   1   |   1   |   >=0 |   1   |
NAME:       |startMark  |CheckChar  |CtrlWord   |frameSerial    |cmdGroup   |cmdCode|paraLen|para   |endMark|
VALUE:      |0x7e       |                                                                           |0x7e   |
*/
#define FRAME_CHAR_SYNC             0x7E    ///< 帧同步符
#define FRAME_CHAR_ESC              0x7D    ///< 帧转义符
#define FRAME_CHAR_RAW_SYNC         0x5E    ///< 帧同步符的裸数据下半部
#define FRAME_CHAR_RAW_ESC          0x5D    ///< 帧转义符的裸数据下半部

#define CHECK_CHAR_OFFSET           0
#define CHECK_CHAR_LEN              1

#define CONTROL_WROD_OFFSET         (CHECK_CHAR_OFFSET +CHECK_CHAR_LEN)
#define CONTROL_WROD_LEN            1

#define FRAME_SERIAL_OFFSET         (CONTROL_WROD_OFFSET +CONTROL_WROD_LEN)
#define FRAME_SERIAL_LEN            1

#define COMMAND_GROUP_OFFSET        (FRAME_SERIAL_OFFSET +FRAME_SERIAL_LEN)
#define COMMAND_GROUP_LEN           1

#define COMMAND_CODE_OFFSET         (COMMAND_GROUP_OFFSET +COMMAND_GROUP_LEN)
#define COMMAND_CODE_LEN            1

#define COMMAND_PARA_LEN_OFFSET     (COMMAND_CODE_OFFSET +COMMAND_CODE_LEN)
#define COMMAND_PARA_LEN_LEN        1

#define COMMAND_PARA_OFFSET         (COMMAND_PARA_LEN_OFFSET +COMMAND_PARA_LEN_LEN)
//#define COMMAND_PARA_LEN


#define FRAME_HEADER                COMMAND_PARA_OFFSET
#define COMMAND_PARA_LEN_MAX        50

#define FRAME_MIN_SIZE              FRAME_HEADER
#define FRAME_MAX_SIZE              (FRAME_HEADER +COMMAND_PARA_LEN_MAX)


/*********************************************************************
 * TYPEDEFS
 */
typedef enum{
    FRAME_LL_DIR_UP     = 0,
    FRAME_LL_DIR_DOWN   = 1,
}FRAME_DIR;

typedef enum{
    FRAME_TYPE_COMMAND          = 0,
    FRAME_TYPE_COMMAND_RESP     = 1,
    FRAME_TYPE_NOTIFY           = 2,
    FRAME_TYPE_NOTIFY_RESP      = 3,
}FRAME_TYPE;

//链路层Ll的头部定义-控制字
typedef struct IwupCtrlWordStruct
{
    unsigned char dir               : 1;        ///< 数据方向：0-上行，从设备向蓝牙模块发送；1-下行，蓝牙模块向设备发送数据
    unsigned char reserved          : 3;        ///< 保留：暂时不用，用0填充
    unsigned char frameType         : 4;        ///< 帧类型：0-命令，蓝牙模块向从设备发送命令；1-命令回应；2-通知，从设备向蓝牙模块发送通知；3-通知回应
} IwupCtrlWord;

//完整帧定义
typedef struct IwupFrameFormatStruct
{
    //uint8_t sync;             // 0x7E帧头
//----链路层Link layer
    uint8_t checkData;          //校验和
    IwupCtrlWord ctrlWord;      //控制字
    uint8_t frameSerial;        //序号
//----应用层Application layer
    uint8_t cmdGroup;           //命令组
    uint8_t cmdCode;            //命令码
    uint8_t paraLen;            //参数长度
    uint8_t para[COMMAND_PARA_LEN_MAX]; //参数

    //uint8_t sync;             // 0x7E帧尾
}__attribute__ ((__packed__)) IwupFrame;    //引用的相关结构体定义，都需要紧凑型定义

/*********************************************************************
 * API FUNCTIONS
 */
void IWUP_frame_init();
void IWUP_frame_send        (FRAME_TYPE type, uint8_t cmdGroup, uint8_t cmdCode, uint8_t* para, uint8_t paraLen, uint8_t portNum);
void IWUP_frame_recieve     (uint8_t* data, uint16_t len);
/*********************************************************************
*********************************************************************/


#endif /* _IWUP_FRAME_H_ */
