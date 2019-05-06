/*
 * ConfigProtocol.h
 *
 *  Created on: 2017��8��8��
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_CONFIG_CONFIGPROTOCOL_H_
#define SOURCE_CONFIG_CONFIGPROTOCOL_H_


#define SOFT_VERSION_BLEONLOCK_S31      0
#define SOFT_VERSION_BLEMODULE_PA       1
#define SOFT_VERSION_LAUNCHPAD          2

#define BOARD_DEFINE_BLEONLOCK_S31      0
#define BOARD_DEFINE_BLEMODULE_PA       1
#define BOARD_DEFINE_LANUCHPAD          2

#define NPI_USE_UART    1

#define BIAS_VOLTAGE    (+277)//(+312)//(+80)//(+176)//(+256)//(+70)//(+256)//(-50)//(+256)  //����ƫ��256uA��У��

#if defined BOARD_BLEMODULE_PA              //(����ģ��4*4)------------------------------------
    #warning "config-1       == BOARD_BLEMODULE_PA"             //0-���ұ���
    #define BOARD_DEFINE            BOARD_DEFINE_BLEMODULE_PA   //1-���Ŷ���
    #define BOARD_MESG              "Board ble module with PA." //2-����Ϣ��ӡ
    #define                         CC2640R2DK_4XS              //3-оƬ��װ
    #define SOFT_VERSION            SOFT_VERSION_BLEMODULE_PA   //4-����汾
    #define                         IWUP_ROLE_WIRELESS_MODULE   //5-IWUPЭ�� ��ɫ
    #define BLE_ADV_ALOW_SLEEP      1   //1                     //6-BLE�㲥��������
    #define UART_ALOW_SLEEP         0   //0                     //7-UART��������
    #define PA_ENABLE               1   //1                     //8-PAʹ��
    #define CONSOLE_EMULATOR        1   //1                     //9-���Կ���̨
    #define _CS_TRACE_LEVEL         7   //7                     //A-��������ȼ�
#elif defined BOARD_LAUNCHPAD               //(����ģ��7*7)------------------------------------
    #warning "config-2         == BOARD_LAUNCHPAD"              //0-���ұ���
    #define BOARD_DEFINE            BOARD_DEFINE_LANUCHPAD      //1-���Ŷ���
    #define BOARD_MESG              "Board launchpad."          //2-����Ϣ��ӡ
    #define                         CC2640R2DK_7XS              //3-оƬ��װ
    #define SOFT_VERSION            SOFT_VERSION_LAUNCHPAD      //4-����汾
    #define                         IWUP_ROLE_WIRELESS_MODULE   //5-IWUPЭ�� ��ɫ
//    #define BLE_ADV_ALOW_SLEEP      1                           //6-BLE�㲥��������
//    #define UART_ALOW_SLEEP         0                           //7-UART��������
    #define PA_ENABLE               0                           //8-PAʹ��
    #define CONSOLE_EMULATOR        1                           //9-���Կ���̨
    #define _CS_TRACE_LEVEL         1                           //A-��������ȼ�
    #define COMPILIE_VERSION        "1-0"
    #define BLE_ROLE_MESG           "Simple Central."
#elif defined BOARD_LAUNCHPAD_NODEBUG       //(������-����ģ��7*7)------------------------------------
    #warning "config-3         == BOARD_LAUNCHPAD_NODEBUG"        //0-���ұ���
    #define BOARD_DEFINE            BOARD_DEFINE_LANUCHPAD      //1-���Ŷ���
    #define BOARD_MESG              "Board launchpad."          //2-����Ϣ��ӡ
    #define                         CC2640R2DK_7XS              //3-оƬ��װ
    #define SOFT_VERSION            SOFT_VERSION_LAUNCHPAD      //4-����汾
    #define                         IWUP_ROLE_WIRELESS_MODULE   //5-IWUPЭ�� ��ɫ
    #define BLE_ADV_ALOW_SLEEP      1                           //6-BLE�㲥��������
    #define UART_ALOW_SLEEP         1                           //7-UART��������
    #define PA_ENABLE               0                           //8-PAʹ��
    #define CONSOLE_EMULATOR        0                           //9-���Կ���̨
    #define _CS_TRACE_LEVEL         0                           //A-��������ȼ�
#elif defined BOARD_BLEONLOCK_S31           //(S31��������4*4)------------------------------------
    #warning "config-4       == BOARD_BLEONLOCK_S31"            //0-���ұ���
    #define BOARD_DEFINE            BOARD_DEFINE_BLEONLOCK_S31  //1-���Ŷ���
    #define BOARD_MESG              "Board on lock S31."        //2-����Ϣ��ӡ
    #define                         CC2640R2DK_4XS              //3-оƬ��װ
    #define SOFT_VERSION            SOFT_VERSION_BLEONLOCK_S31  //4-����汾
    #define                         IWUP_ROLE_WIRELESS_MODULE   //5-IWUPЭ�� ��ɫ
//    #define BLE_ADV_ALOW_SLEEP      1   //1                     //6-BLE�㲥��������
//    #define UART_ALOW_SLEEP         0   //0                     //7-UART��������
    #define PA_ENABLE               0   //0                     //8-PAʹ��
    #define CONSOLE_EMULATOR        1   //1                     //9-���Կ���̨
    #define _CS_TRACE_LEVEL         7   //7                     //A-��������ȼ�
    #define COMPILIE_VERSION        "1-1"
#elif defined BOARD_LAUNCHPAD_IWUP_LOCKBOARD  //(ģ������Ĵ���)------------------------------------
    #warning "config-7       == BOARD_LAUNCHPAD_IWUP_LOCKBOARD" //0-���ұ���
    #define BOARD_DEFINE            BOARD_DEFINE_LANUCHPAD      //1-���Ŷ���
    #define BOARD_MESG              "Board launchpad."          //2-����Ϣ��ӡ
    #define                         CC2640R2DK_7XS              //3-оƬ��װ
    #define SOFT_VERSION            SOFT_VERSION_LAUNCHPAD      //4-����汾
    #define                         IWUP_ROLE_LOCK_BOARD        //5-IWUPЭ�� ��ɫ
    #define BLE_ADV_ALOW_SLEEP      1                           //6-BLE�㲥��������
    #define UART_ALOW_SLEEP         0                           //7-UART��������
    #define PA_ENABLE               0                           //8-PAʹ��
    #define CONSOLE_EMULATOR        1                           //9-���Կ���̨
    #define _CS_TRACE_LEVEL         7                           //A-��������ȼ�
    #define COMPILIE_VERSION        "1-2"
#else
    #error "BOARD NOT DEFINE!"
#endif


#endif /* SOURCE_CONFIG_CONFIGPROTOCOL_H_ */
