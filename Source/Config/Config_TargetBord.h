/*
 * Config_TargetBord.h
 *
 *  Created on: 2018��9��27��
 *      Author: zheng
 */

#ifndef SOURCE_CONFIG_CONFIG_TARGETBORD_H_
#define SOURCE_CONFIG_CONFIG_TARGETBORD_H_

#define TARGET_BOARD_DEVICE_FACTORY                 0x01

#define TARGET_BOARD_DEVICE_MODEL_S31               0x01//��ָ�ơ���������ģ��
#define TARGET_BOARD_DEVICE_MODEL_S31X              0x02//��ָ�ơ�������ģ��
#define TARGET_BOARD_DEVICE_MODEL_D3111             0x03//��ָ�ơ���������ģ��
#define TARGET_BOARD_DEVICE_MODEL_D3151             0x04//��ָ�ơ�������ģ��
#define TARGET_BOARD_DEVICE_MODEL_D3100             0x05//����ָ�ơ���������ģ��

                                                        //4֧��ָ��-3֧�ַ��˸澯-2֧�ֳ���-1֧�ֵ�������-0֧�ֻ�е�������
#define TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_S31      0x1f//  1           1           1           1               1
#define TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_S31X     0x1f//  1           1           1           1               1
#define TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_D3111    0x1f//  1           1           1           1               1
#define TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_D3151    0x1f//  1           1           1           1               1
#define TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_D3100    0x0f//  0           1           1           1               1

                                                        //֧������
#define TARGET_BOARD_DEVICE_GATEWAY_TYPE_S31        0x00//  ��֧��
#define TARGET_BOARD_DEVICE_GATEWAY_TYPE_S31X       0x01//  ֧��ZLG-zigbee
#define TARGET_BOARD_DEVICE_GATEWAY_TYPE_D3111      0x00//  ��֧��
#define TARGET_BOARD_DEVICE_GATEWAY_TYPE_D3151      0x01//  ֧��ZLG-zigbee
#define TARGET_BOARD_DEVICE_GATEWAY_TYPE_D3100      0x00//  ��֧��


#endif /* SOURCE_CONFIG_CONFIG_TARGETBORD_H_ */
