/*
 * Aes.h
 *
 *  Created on: 2017��11��29��
 *      Author: zheng
 */

#ifndef AES_H_
#define AES_H_


#include <stdint.h>
//AES����
void AES_Test(void);
//----------��װ���ļӽ��ܷ���----��ʼ������Ĭ��ȫ0x00----
void AES_EncJiaMi(uint8_t* pPlain, uint8_t* pCipher,uint32_t len, uint8_t* pKey);
void AES_DecJieMi(uint8_t* pPlain, uint8_t* pCipher, uint32_t len, uint8_t* pKey);

#endif /* AES_H_ */
