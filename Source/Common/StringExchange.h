#ifndef _STRING_EXCHANGE_
#define _STRING_EXCHANGE_

#include "Common\Types.h"

unsigned int	StringToUint32	(char *str);
char			Uint32ToString	(unsigned int uint32Data,	char *str							);

unsigned int	StringToHex		(char *str,					unsigned char length				);
char			HexToString     (unsigned int hexData, unsigned char hexBitNum,char *str);

char			StringToHexGroup(char *str,					unsigned char len_str,	char *hex	);
char            HexGroupToString(uint8_t *hex,unsigned char hexNum,uint8_t *str, char separator);
unsigned        bcd2bin(unsigned char val);
unsigned char   bin2bcd(unsigned val);

uint8_t         HexString2Dec(uint8_t data);
uint8_t         Dec2HexString(uint8_t data);
Uint32          DecAssicString2Hex(Uint8* Data,Uint8 Len);
void            Hex2DecAssicString(Uint32 Data,Uint8 Len,Uint8* DecSrting);

#endif //_STRING_EXCHANGE_
