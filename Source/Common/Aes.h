/*
 * Aes.h
 *
 *  Created on: 2017年11月29日
 *      Author: zheng
 */

#ifndef AES_H_
#define AES_H_


#include <stdint.h>
//AES测试
void AES_Test(void);
//----------封装过的加解密方法----初始化向量默认全0x00----
void AES_EncJiaMi(uint8_t* pPlain, uint8_t* pCipher,uint32_t len, uint8_t* pKey);
void AES_DecJieMi(uint8_t* pPlain, uint8_t* pCipher, uint32_t len, uint8_t* pKey);

#endif /* AES_H_ */
