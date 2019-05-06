/*
 * IWUP_frame.h
 *
 *  Created on: 2017��12��19��
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
#define FRAME_CHAR_SYNC             0x7E    ///< ֡ͬ����
#define FRAME_CHAR_ESC              0x7D    ///< ֡ת���
#define FRAME_CHAR_RAW_SYNC         0x5E    ///< ֡ͬ�������������°벿
#define FRAME_CHAR_RAW_ESC          0x5D    ///< ֡ת������������°벿

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

//��·��Ll��ͷ������-������
typedef struct IwupCtrlWordStruct
{
    unsigned char dir               : 1;        ///< ���ݷ���0-���У����豸������ģ�鷢�ͣ�1-���У�����ģ�����豸��������
    unsigned char reserved          : 3;        ///< ��������ʱ���ã���0���
    unsigned char frameType         : 4;        ///< ֡���ͣ�0-�������ģ������豸�������1-�����Ӧ��2-֪ͨ�����豸������ģ�鷢��֪ͨ��3-֪ͨ��Ӧ
} IwupCtrlWord;

//����֡����
typedef struct IwupFrameFormatStruct
{
    //uint8_t sync;             // 0x7E֡ͷ
//----��·��Link layer
    uint8_t checkData;          //У���
    IwupCtrlWord ctrlWord;      //������
    uint8_t frameSerial;        //���
//----Ӧ�ò�Application layer
    uint8_t cmdGroup;           //������
    uint8_t cmdCode;            //������
    uint8_t paraLen;            //��������
    uint8_t para[COMMAND_PARA_LEN_MAX]; //����

    //uint8_t sync;             // 0x7E֡β
}__attribute__ ((__packed__)) IwupFrame;    //���õ���ؽṹ�嶨�壬����Ҫ�����Ͷ���

/*********************************************************************
 * API FUNCTIONS
 */
void IWUP_frame_init();
void IWUP_frame_send        (FRAME_TYPE type, uint8_t cmdGroup, uint8_t cmdCode, uint8_t* para, uint8_t paraLen, uint8_t portNum);
void IWUP_frame_recieve     (uint8_t* data, uint16_t len);
/*********************************************************************
*********************************************************************/


#endif /* _IWUP_FRAME_H_ */
