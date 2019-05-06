#include<string.h>
#include"StringExchange.h"


/* ----------------------------------------------------------------------
*|function:	StringToUint32
*  ----------------------------------------------------------------------
*|			|Type			|Example	|Min	|Max			|
*  ----------------------------------------------------------------------
*| input	|char *str		|"115200"	|"0"	|"4294967295"	|
*  ----------------------------------------------------------------------
*| return	|unsigned int	|115200u	|0u		| 4294967295u	|
*  ----------------------------------------------------------------------*/
unsigned int StringToUint32(char *str){
	unsigned char i = 0;
	unsigned int ret_num = 0;
	for(i = 0; i < strlen(str); i++){
		ret_num *= 10;
		ret_num += str[i] - '0';
	}
	return ret_num;
}

/* ----------------------------------------------------------------------
*|function:	Uint32ToString
*  ----------------------------------------------------------------------
*|			|Type						|Example	|Min	|Max			|
*  ----------------------------------------------------------------------
*| input	|unsigned int uint32Data	|115200u	|0u		|4294967295u	|
*  ----------------------------------------------------------------------
*| return	|char *str					|"115200\0"	|"0\0"	|"4294967295" 	|
*  ----------------------------------------------------------------------*/
char Uint32ToString(unsigned int uint32Data,char *str){
	unsigned int temp_num = 1000000000;
	char i = 0;
	char j = 0;
	for(i = 0; i<10 ; i++){
		if(0 != uint32Data/temp_num || 0 != j){
			str[j] = uint32Data/temp_num + '0';
			j++;
		}
		uint32Data %= temp_num;
		temp_num /= 10;
	}
	str[j] = '\0';
	return 0;
}



/* ----------------------------------------------------------------------
*|function:	StringToHex
*  ----------------------------------------------------------------------
*|			|Type			|Example	|Min	|Max			|
*  ----------------------------------------------------------------------
*| input	|char *str		|"5AEB"		|"0"	|"FFFFFFFF"		|
*  ----------------------------------------------------------------------
*|length	|unsigned char	|0:??strlen?§Ø?		|???????ÕÇ???? <255
*  ----------------------------------------------------------------------
*| return	|unsigned int	|0x5AEB		|0x00	| 0xFFFFFFFF	|
*  ----------------------------------------------------------------------*/
unsigned int StringToHex(char *str,unsigned char length){
	unsigned char	i = 0;
	unsigned int	ret_num = 0;
	/*??????????0?????????strlen?§Ø???????*/
	if(0 == length){
		length = strlen(str);
	}
	for(i = 0; i < length; i++){
		ret_num *= 16;
		if(str[i] >= 'A'&&str[i] <= 'F'){
			ret_num += (str[i] - 'A' + 0x0A);
		}else if(str[i] >= 'a'&&str[i] <= 'f'){
			ret_num += str[i] - 'a' + 0x0A;
		}else if(str[i] >= '0'&&str[i] <= '9'){
			ret_num += str[i] - '0';			
		}
	}
	return ret_num;
}

/* ----------------------------------------------------------------------
*|function:	HexToString
*  ----------------------------------------------------------------------
*|			|Type					|Example	|Min	|Max			|
*  ----------------------------------------------------------------------
*|input		|unsigned int hexData	|0x5AEB		|0		|0xffffffff		|
*  ----------------------------------------------------------------------
*|output	|char *str				|"5AEB"		|"0"	|"FFFFFFFF\0"	|
*  ----------------------------------------------------------------------
*| return	|char					|0|
*  ----------------------------------------------------------------------*/
char HexToString(unsigned int hexData, unsigned char hexBitNum,char *str){
	char result = 0;
    char bit = 0;
    bit = hexBitNum;

	for(; bit>0; bit--){
        result = (hexData>>(4*(bit-1))) & 0xf;

        if(result >= 0 && result <= 9){
            str[hexBitNum -bit] = result + '0';
        }else if(result >= 10 && result <= 15){
            str[hexBitNum -bit] = result - 10 + 'A';
        }
	}
    return hexBitNum;
}

/* ----------------------------------------------------------------------
*|function:	StringToHexGroup
*  ----------------------------------------------------------------------
*|			|Type					|Example					|
*  ----------------------------------------------------------------------
*| input1	|char *str				|"5AEB8C"					|???????????????????
*  ----------------------------------------------------------------------
*| input2	|unsigned char len_str	|6							|??????????????????????????????????hex,????0????strlen????str????
*  ----------------------------------------------------------------------
*| input3	|char *hex				|char hex[3]:????›¥???	|
*  ----------------------------------------------------------------------
*| output2	|char *hex				|hex[3] = {0x5A,0XEB,0X8C}	|
*  ----------------------------------------------------------------------
*| return	|char					|0:SUCCESS	|-1:??????????|
*  ----------------------------------------------------------------------*/
char StringToHexGroup(char *str, unsigned char len_str, char *hex){
	unsigned char i = 0;
	if(0 != len_str%2){
		return (-1);
	}
	if(0 == len_str){
		len_str = strlen(str);
	}

	for(i = 0;i < len_str/2; i++){
		hex[i] = (char)StringToHex(str +2*i,2);
	}
	return 0;
}

/* ----------------------------------------------------------------------
*|function:	HexGroupToString
*  ----------------------------------------------------------------------
*|			|Type					|Example					|
*  ----------------------------------------------------------------------
*| input1	|char *str				|"5AEB8C"					|???????????????????
*  ----------------------------------------------------------------------
*| input2	|unsigned char len_str	|6							|??????????????????????????????????hex,????0????strlen????str????
*  ----------------------------------------------------------------------
*| input3	|char *hex				|char hex[3]:????›¥???	|
*  ----------------------------------------------------------------------
*| output2	|char *hex				|hex[3] = {0x5A,0XEB,0X8C}	|
*  ----------------------------------------------------------------------
*| return	|char					|0:SUCCESS	|-1:??????????|
*  ----------------------------------------------------------------------*/
char HexGroupToString(uint8_t *hex,unsigned char hexNum,uint8_t *str, char separator){
	unsigned char i;

	for(i = 0;i < hexNum; i++){
		HexToString((char)hex[i], 2,(char*)str + i*3);
        str[i*3+2] = separator;
	}
	return 0;
}


//BCD??????????
unsigned bcd2bin(unsigned char val)
{
	return (val & 0x0f) + (val >> 4) * 10;
}

//????????BCD??
unsigned char bin2bcd(unsigned val)
{
	return ((val / 10) << 4) + val % 10;
}


uint8_t HexString2Dec(uint8_t data)
{
    uint8_t decimal;
    //@todo ¦Ä???????§µ?î•???????????????
    decimal = (data&0xF) + 10*((data>>4)&0xF);
    
    return decimal;
}
uint8_t Dec2HexString(uint8_t data)
{
    uint8_t HexString;

    HexString = (((data%10)&0xF)) + (((data/10)<<4)&0xF0);
    return HexString;
}
Uint32 DecAssicString2Hex(Uint8* Data,Uint8 Len)
{
	int i;
	Uint32 Result=0;
	for(i=0;i<Len;i++)
	{
		Result = Result*10 +(Data[i]-0x30);
	}
	return Result;
}
void Hex2DecAssicString(Uint32 Data,Uint8 Len,Uint8* DecSrting)
{
	int i;
	Uint32 DataTmp;
	DataTmp =Data;
	for(i=0;i<Len;i++)
	{
		DecSrting[Len-1-i] = DataTmp%10 + 0x30;
		DataTmp = DataTmp/10;
	}
}
