//#ifndef __FLASH_H
//#define __FLASH_H
////#include "em_chip.h"
////////////////////////////////////////////////////////////////////////////////////
////������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
////ALIENTEKս��STM32������V3
////W25QXX ��������
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////��������:2015/1/15
////�汾��V1.0
////��Ȩ���У�����ؾ���
////Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
////All rights reserved
////////////////////////////////////////////////////////////////////////////////////
//
//
//// SPI�����ٶ�����
//#define SPI_SPEED_2   		0
//#define SPI_SPEED_4   		1
//#define SPI_SPEED_8   		2
//#define SPI_SPEED_16  		3
//#define SPI_SPEED_32 		4
//#define SPI_SPEED_64 		5
//#define SPI_SPEED_128 		6
//#define SPI_SPEED_256 		7
//
//
//
//void SPI2_Init(void);			 //��ʼ��SPI2��
////void SPI2_SetSpeed(unsigned char SpeedSet); //����SPI2�ٶ�
//unsigned char SPI2_ReadWriteByte(unsigned char TxData);//SPI2���߶�дһ���ֽ�
//
//
////W25Xϵ��/Qϵ��оƬ�б�
////W25Q80  ID  0XEF13
////W25Q16  ID  0XEF14
////W25Q32  ID  0XEF15
////W25Q64  ID  0XEF16
////W25Q128 ID  0XEF17
//#define W25Q80 	0XEF13
//#define W25Q16 	0XEF14
//#define W25Q32 	0XEF15
//#define W25Q64 	0XEF16
//#define W25Q128	0XEF17
//
//#define GDQ16	0xC814
//
//extern unsigned int W25QXX_TYPE;					//����W25QXXоƬ�ͺ�
//
////#define	W25QXX_CS 		PBout(12)  		//W25QXX��Ƭѡ�ź�
//
////////////////////////////////////////////////////////////////////////////////////
////ָ���
//#define W25X_WriteEnable		0x06
//#define W25X_WriteDisable		0x04
//#define W25X_ReadStatusReg		0x05
//#define W25X_WriteStatusReg		0x01
//#define W25X_ReadData			0x03
//#define W25X_FastReadData		0x0B
//#define W25X_FastReadDual		0x3B
//#define W25X_PageProgram		0x02
//#define W25X_BlockErase			0xD8
//#define W25X_SectorErase		0x20
//#define W25X_ChipErase			0xC7
//#define W25X_PowerDown			0xB9
//#define W25X_ReleasePowerDown	0xAB
//#define W25X_DeviceID			0xAB
//#define W25X_ManufactDeviceID	0x90
//#define W25X_JedecDeviceID		0x9F
//
//void W25QXX_Init(void);
//unsigned int  W25QXX_ReadID(void);  	    		//��ȡFLASH ID
//unsigned char W25QXX_ReadSR(void);        		//��ȡ״̬�Ĵ���
//void W25QXX_Write_SR(unsigned char sr);  			//д״̬�Ĵ���
//void W25QXX_Write_Enable(void);  		//дʹ��
//void W25QXX_Write_Disable(void);		//д����
//void W25QXX_Write_NoCheck(unsigned char* pBuffer,unsigned long WriteAddr,unsigned int NumByteToWrite);
//void W25QXX_Read(unsigned char* pBuffer,unsigned long ReadAddr,unsigned int NumByteToRead);   //��ȡflash
//void W25QXX_Write(unsigned char* pBuffer,unsigned long WriteAddr,unsigned int NumByteToWrite);//д��flash
//void W25QXX_Erase_Chip(void);    	  	//��Ƭ����
//void W25QXX_Erase_Sector(unsigned long Dst_Addr);	//��������
//void W25QXX_Wait_Busy(void);           	//�ȴ�����
//void W25QXX_PowerDown(void);        	//�������ģʽ
//void W25QXX_WAKEUP(void);				//����
//#endif
//
//
//
