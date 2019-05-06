/*
 * Aes.c
 *
 *  Created on: 2017年11月29日
 *      Author: zheng
 */

#include "AesCore.h"
#include "stdio.h"
#include "Trace.h"

//--------------------------封装过的加解密
//初始化向量
static uint8_t initIV[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void AES_EncJiaMi(uint8_t* pPlain, uint8_t* pCipher, uint32_t len, uint8_t* pKey)
{
	uint8_t i;
	AES_Init(pKey);
	for (i = 0; i < len / 16; i++)
		AES_Encrypt(pPlain + i * 16, pCipher + i * 16, 16, initIV);	//加密
}

void AES_DecJieMi(uint8_t* pPlain, uint8_t* pCipher, uint32_t len, uint8_t* pKey)
{
	uint8_t i;
	AES_Init(pKey);
	for (i = 0; i < len / 16; i++)
		AES_Decrypt(pPlain + i * 16, pCipher + i * 16, 16, initIV);	//解密
}

void AES_Test()
{
	unsigned char pKey[16] = "This is a key--";

	unsigned char originalText[16] = "1234567890abcde";
	unsigned char cipherText[16];
	unsigned char decryptText[16];

	TRACE_DEBUG("AES test begain.\n");
	TRACE_DEBUG("Original text is:                   %s.\n", originalText);
	TRACE_DEBUG("key is:                             %s.\n", pKey);

    AES_EncJiaMi(originalText, cipherText, 16, pKey);

	TRACE_DEBUG("after encryption the cipherText is:\t", cipherText);
	TRACE_DEBUG_HEXGROUP(cipherText, 16, ' ');

	AES_DecJieMi(decryptText, cipherText, 16, pKey);

	TRACE_DEBUG("after Decrypt the decryptText is:\t", decryptText);
	TRACE_DEBUG_HEXGROUP(decryptText, 16, ' ');
}
