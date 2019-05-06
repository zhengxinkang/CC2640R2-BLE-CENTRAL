/*
 * Config_TargetBord.h
 *
 *  Created on: 2018年9月27日
 *      Author: zheng
 */

#ifndef SOURCE_CONFIG_CONFIG_TARGETBORD_H_
#define SOURCE_CONFIG_CONFIG_TARGETBORD_H_

#define TARGET_BOARD_DEVICE_FACTORY                 0x01

#define TARGET_BOARD_DEVICE_MODEL_S31               0x01//带指纹、不带网关模块
#define TARGET_BOARD_DEVICE_MODEL_S31X              0x02//带指纹、带网关模块
#define TARGET_BOARD_DEVICE_MODEL_D3111             0x03//带指纹、不带网关模块
#define TARGET_BOARD_DEVICE_MODEL_D3151             0x04//带指纹、带网关模块
#define TARGET_BOARD_DEVICE_MODEL_D3100             0x05//不带指纹、不带网关模块

                                                        //4支持指纹-3支持防撬告警-2支持常开-1支持电子锁定-0支持机械反锁检测
#define TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_S31      0x1f//  1           1           1           1               1
#define TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_S31X     0x1f//  1           1           1           1               1
#define TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_D3111    0x1f//  1           1           1           1               1
#define TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_D3151    0x1f//  1           1           1           1               1
#define TARGET_BOARD_DEVICE_FUNC_ATTRIBUTE_D3100    0x0f//  0           1           1           1               1

                                                        //支持网关
#define TARGET_BOARD_DEVICE_GATEWAY_TYPE_S31        0x00//  不支持
#define TARGET_BOARD_DEVICE_GATEWAY_TYPE_S31X       0x01//  支持ZLG-zigbee
#define TARGET_BOARD_DEVICE_GATEWAY_TYPE_D3111      0x00//  不支持
#define TARGET_BOARD_DEVICE_GATEWAY_TYPE_D3151      0x01//  支持ZLG-zigbee
#define TARGET_BOARD_DEVICE_GATEWAY_TYPE_D3100      0x00//  不支持


#endif /* SOURCE_CONFIG_CONFIG_TARGETBORD_H_ */
