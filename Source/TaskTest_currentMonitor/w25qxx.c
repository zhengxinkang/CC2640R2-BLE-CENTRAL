//#include "w25qxx.h"
////#include "delay.h"
////#include "usart.h"
////#include "em_cmu.h"
////#include "stdio.h"
////#include "smartlockconfig.h"
////#include "debug.h"
//#include <ti/drivers/PIN.h>
//#include "Board.h"
//#include "Types.h"
//#include "Trace.h"
//#include "BF_Util.h"
//\
//#define delay_us    BF_taskSleepUs
//
//static PIN_Handle hFlashSpiPin = NULL;
//static PIN_State pinFlashSpiState;
//
//static PIN_Config FlashSpiPinTable[] =
//{
// BOARD_PORT_FLASH_SPI_MOSI   |PIN_GPIO_OUTPUT_EN     |PIN_GPIO_HIGH   |PIN_PUSHPULL   |PIN_DRVSTR_MAX,
// BOARD_PORT_FLASH_SPI_CLK    |PIN_GPIO_OUTPUT_EN     |PIN_GPIO_HIGH   |PIN_PUSHPULL   |PIN_DRVSTR_MAX,
// BOARD_PORT_FLASH_SPI_MISO   |PIN_INPUT_EN           |PIN_PULLUP,
// BOARD_PORT_FLASH_SPI_CSn    |PIN_GPIO_OUTPUT_EN     |PIN_GPIO_HIGH   |PIN_PUSHPULL   |PIN_DRVSTR_MAX,
// PIN_TERMINATE//��ֹ��
//};
//
//
//#define GPIO_FLASH_SPI_MOSI     BOARD_PORT_FLASH_SPI_MOSI
//#define GPIO_FLASH_SPI_CLK      BOARD_PORT_FLASH_SPI_CLK
//#define GPIO_FLASH_SPI_MISO     BOARD_PORT_FLASH_SPI_MISO
//#define GPIO_FLASH_SPI_CS       BOARD_PORT_FLASH_SPI_CSn
//
//#define BIT_FLASH_SPI_CS        1
//#define BIT_FLASH_SPI_CLK       1
//#define BIT_FLASH_SPI_MOSI      1
//#define BIT_FLASH_SPI_MISO      1
//
//void GPIO_PinOutClear(PIN_Id pinId, uint32_t reserved)
//{
//    PIN_setOutputValue(hFlashSpiPin, pinId, PORT_VALUE_LOW);
//}
//
//void GPIO_PinOutSet(PIN_Id pinId, uint32_t reserved)
//{
//    PIN_setOutputValue(hFlashSpiPin, pinId, PORT_VALUE_HIGH);
//}
//
//uint32_t GPIO_PinInGet(PIN_Id pinId, uint32_t reserved)
//{
//    return PIN_getInputValue(pinId);
//}
//
////-------------
//unsigned int W25QXX_TYPE=W25Q128;	//Ĭ����W25Q128
//
////4KbytesΪһ��Sector
////16������Ϊ1��Block
////W25Q128
////����Ϊ16M�ֽ�,����128��Block,4096��Sector
//
////��ʼ��SPI FLASH��IO��
//void W25QXX_Init(void)
//{
//	SPI2_Init();		   		//��ʼ��SPI
//    W25QXX_WAKEUP();
//	W25QXX_TYPE=W25QXX_ReadID();//��ȡFLASH ID.
//    TRACE_CODE("W25QXX_TYPE = %x\n",W25QXX_TYPE);
//}
//
////��ȡW25QXX��״̬�Ĵ���
////BIT7  6   5   4   3   2   1   0
////SPR   RV  TB BP2 BP1 BP0 WEL BUSY
////SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
////TB,BP2,BP1,BP0:FLASH����д��������
////WEL:дʹ������
////BUSY:æ���λ(1,æ;0,����)
////Ĭ��:0x00
//unsigned char W25QXX_ReadSR(void)
//{
//	unsigned char byte=0;
//	GPIO_PinOutClear(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                            //ʹ������
//	SPI2_ReadWriteByte(W25X_ReadStatusReg); //���Ͷ�ȡ״̬�Ĵ�������
//	byte=SPI2_ReadWriteByte(0Xff);          //��ȡһ���ֽ�
//	GPIO_PinOutSet(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                            //ȡ��Ƭѡ
//	return byte;
//}
////дW25QXX״̬�Ĵ���
////ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
//void W25QXX_Write_SR(unsigned char sr)
//{
//	GPIO_PinOutClear(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                            //ʹ������
//	SPI2_ReadWriteByte(W25X_WriteStatusReg);//����дȡ״̬�Ĵ�������
//	SPI2_ReadWriteByte(sr);               	//д��һ���ֽ�
//	GPIO_PinOutSet(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                            //ȡ��Ƭѡ
//}
////W25QXXдʹ��
////��WEL��λ
//void W25QXX_Write_Enable(void)
//{
//	GPIO_PinOutClear(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                          	//ʹ������
//    SPI2_ReadWriteByte(W25X_WriteEnable); 	//����дʹ��
//	GPIO_PinOutSet(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                           	//ȡ��Ƭѡ
//}
////W25QXXд��ֹ
////��WEL����
//void W25QXX_Write_Disable(void)
//{
//	GPIO_PinOutClear(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                            //ʹ������
//    SPI2_ReadWriteByte(W25X_WriteDisable);  //����д��ָֹ��
//	GPIO_PinOutSet(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                            //ȡ��Ƭѡ
//}
////��ȡоƬID
////����ֵ����:
////0XEF13,��ʾоƬ�ͺ�ΪW25Q80
////0XEF14,��ʾоƬ�ͺ�ΪW25Q16
////0XEF15,��ʾоƬ�ͺ�ΪW25Q32
////0XEF16,��ʾоƬ�ͺ�ΪW25Q64
////0XEF17,��ʾоƬ�ͺ�ΪW25Q128
//unsigned int W25QXX_ReadID(void)
//{
//	unsigned int Temp = 0;
//	GPIO_PinOutClear(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);
//	SPI2_ReadWriteByte(0x90);//���Ͷ�ȡID����
//	SPI2_ReadWriteByte(0x00);
//	SPI2_ReadWriteByte(0x00);
//	SPI2_ReadWriteByte(0x00);
//	Temp|=SPI2_ReadWriteByte(0xFF)<<8;
//	Temp|=SPI2_ReadWriteByte(0xFF);
//	GPIO_PinOutSet(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);
//	return Temp;
//}
////��ȡSPI FLASH
////��ָ����ַ��ʼ��ȡָ�����ȵ�����
////pBuffer:���ݴ洢��
////ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
////NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
//void W25QXX_Read(unsigned char* pBuffer,unsigned long ReadAddr,unsigned int NumByteToRead)
//{
// 	unsigned int i;
//	GPIO_PinOutClear(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                            	//ʹ������
//    SPI2_ReadWriteByte(W25X_ReadData);         	//���Ͷ�ȡ����
//    SPI2_ReadWriteByte((unsigned char)((ReadAddr)>>16));  	//����24bit��ַ
//    SPI2_ReadWriteByte((unsigned char)((ReadAddr)>>8));
//    SPI2_ReadWriteByte((unsigned char)ReadAddr);
//    for(i=0;i<NumByteToRead;i++)
//	{
//        pBuffer[i]=SPI2_ReadWriteByte(0XFF);   	//ѭ������
//    }
//	GPIO_PinOutSet(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);
//}
////SPI��һҳ(0~65535)��д������256���ֽڵ�����
////��ָ����ַ��ʼд�����256�ֽڵ�����
////pBuffer:���ݴ洢��
////WriteAddr:��ʼд��ĵ�ַ(24bit)
////NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
//void W25QXX_Write_Page(unsigned char* pBuffer,unsigned long WriteAddr,unsigned int NumByteToWrite)
//{
// 	unsigned int i;
//    W25QXX_Write_Enable();                  	//SET WEL
//	GPIO_PinOutClear(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                            	//ʹ������
//    SPI2_ReadWriteByte(W25X_PageProgram);      	//����дҳ����
//    SPI2_ReadWriteByte((unsigned char)((WriteAddr)>>16)); 	//����24bit��ַ
//    SPI2_ReadWriteByte((unsigned char)((WriteAddr)>>8));
//    SPI2_ReadWriteByte((unsigned char)WriteAddr);
//    for(i=0;i<NumByteToWrite;i++)SPI2_ReadWriteByte(pBuffer[i]);//ѭ��д��
//	GPIO_PinOutSet(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                            	//ȡ��Ƭѡ
//	W25QXX_Wait_Busy();					   		//�ȴ�д�����
//}
////�޼���дSPI FLASH
////����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
////�����Զ���ҳ����
////��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
////pBuffer:���ݴ洢��
////WriteAddr:��ʼд��ĵ�ַ(24bit)
////NumByteToWrite:Ҫд����ֽ���(���65535)
////CHECK OK
//void W25QXX_Write_NoCheck(unsigned char* pBuffer,unsigned long WriteAddr,unsigned int NumByteToWrite)
//{
//	unsigned int pageremain;
//	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���
//	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
//	while(1)
//	{
//		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
//		if(NumByteToWrite==pageremain)break;//д�������
//	 	else //NumByteToWrite>pageremain
//		{
//			pBuffer+=pageremain;
//			WriteAddr+=pageremain;
//
//			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
//			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
//			else pageremain=NumByteToWrite; 	  //����256���ֽ���
//		}
//	};
//}
////дSPI FLASH
////��ָ����ַ��ʼд��ָ�����ȵ�����
////�ú�������������!
////pBuffer:���ݴ洢��
////WriteAddr:��ʼд��ĵ�ַ(24bit)
////NumByteToWrite:Ҫд����ֽ���(���65535)
////unsigned char W25QXX_BUFFER[4096];
//void W25QXX_Write(unsigned char* pBuffer,unsigned long WriteAddr,unsigned int NumByteToWrite)
//{
////	unsigned long secpos;
////	unsigned int secoff;
////	unsigned int secremain;
//// 	unsigned int i;
////	unsigned char * W25QXX_BUF;
////   	W25QXX_BUF=W25QXX_BUFFER;
//// 	secpos=WriteAddr/4096;//������ַ
////	secoff=WriteAddr%4096;//�������ڵ�ƫ��
////	secremain=4096-secoff;//����ʣ��ռ��С
//// 	//PrintDebug("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
//// 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
////	while(1)
////	{
////		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
////		for(i=0;i<secremain;i++)//У������
////		{
////			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����
////		}
////		if(i<secremain)//��Ҫ����
////		{
////			W25QXX_Erase_Sector(secpos);		//�����������
////			for(i=0;i<secremain;i++)	   		//����
////			{
////				W25QXX_BUF[i+secoff]=pBuffer[i];
////			}
////			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������
////
////		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
////		if(NumByteToWrite==secremain)break;//д�������
////		else//д��δ����
////		{
////			secpos++;//������ַ��1
////			secoff=0;//ƫ��λ��Ϊ0
////
////		   	pBuffer+=secremain;  				//ָ��ƫ��
////			WriteAddr+=secremain;				//д��ַƫ��
////		   	NumByteToWrite-=secremain;			//�ֽ����ݼ�
////			if(NumByteToWrite>4096)secremain=4096;//��һ����������д����
////			else secremain=NumByteToWrite;		//��һ����������д����
////		}
////	};
//}
////��������оƬ
////�ȴ�ʱ�䳬��...
//void W25QXX_Erase_Chip(void)
//{
//    W25QXX_Write_Enable();                 	 	//SET WEL
//    W25QXX_Wait_Busy();
//  	GPIO_PinOutClear(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                            	//ʹ������
//    SPI2_ReadWriteByte(W25X_ChipErase);        	//����Ƭ��������
//	GPIO_PinOutSet(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                            	//ȡ��Ƭѡ
//	W25QXX_Wait_Busy();   				   		//�ȴ�оƬ��������
//}
////����һ������
////Dst_Addr:������ַ ����ʵ����������
////����һ��ɽ��������ʱ��:150ms
//void W25QXX_Erase_Sector(unsigned long Dst_Addr)
//{
//	//����falsh�������,������
//// 	PrintDebug("fe:%x\r\n",Dst_Addr);
// 	Dst_Addr*=4096;
//    W25QXX_Write_Enable();                  	//SET WEL
//    W25QXX_Wait_Busy();
//  	GPIO_PinOutClear(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                            	//ʹ������
//    SPI2_ReadWriteByte(W25X_SectorErase);      	//������������ָ��
//    SPI2_ReadWriteByte((unsigned char)((Dst_Addr)>>16));  	//����24bit��ַ
//    SPI2_ReadWriteByte((unsigned char)((Dst_Addr)>>8));
//    SPI2_ReadWriteByte((unsigned char)Dst_Addr);
//	GPIO_PinOutSet(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                             	//ȡ��Ƭѡ
//    W25QXX_Wait_Busy();   				   		//�ȴ��������
//}
////�ȴ�����
//void W25QXX_Wait_Busy(void)
//{
//	while((W25QXX_ReadSR()&0x01)==0x01);                    // �ȴ�BUSYλ���
//}
////�������ģʽ
//void W25QXX_PowerDown(void)
//{
//  	GPIO_PinOutClear(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                         //ʹ������
//    SPI2_ReadWriteByte(W25X_PowerDown);                     //���͵�������
//	GPIO_PinOutSet(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);                           //ȡ��Ƭѡ
//    delay_us(3);                                            //�ȴ�TPD
//}
////����
//void W25QXX_WAKEUP(void)
//{
//  	GPIO_PinOutClear(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);  //ʹ������
//    SPI2_ReadWriteByte(W25X_ReleasePowerDown);              //  send W25X_PowerDown command 0xAB
//	GPIO_PinOutSet(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);    //ȡ��Ƭѡ
//    delay_us(3);                                            //�ȴ�TRES1
//}
//
////SPI2 ��дһ���ֽ�
////TxData:Ҫд����ֽ�
////����ֵ:��ȡ�����ֽ�
//unsigned char SPI2_ReadWriteByte(unsigned char TxData)
//{
//	unsigned char tr_data = 0;
//	int i=0;
//
//	GPIO_PinOutClear(GPIO_FLASH_SPI_CLK, BIT_FLASH_SPI_CLK);
//	for(i=7;i>=0;i--)
//	{
//		GPIO_PinOutClear(GPIO_FLASH_SPI_CLK, BIT_FLASH_SPI_CLK);
//		if(TxData&(1<<i))
//		{
//			GPIO_PinOutSet(GPIO_FLASH_SPI_MOSI, BIT_FLASH_SPI_MOSI);
//		}else
//		{
//			GPIO_PinOutClear(GPIO_FLASH_SPI_MOSI, BIT_FLASH_SPI_MOSI);
//		}
//
//		delay_us(1);
//		GPIO_PinOutSet(GPIO_FLASH_SPI_CLK, BIT_FLASH_SPI_CLK);
//		tr_data=tr_data<<1;
//		tr_data|=GPIO_PinInGet(GPIO_FLASH_SPI_MISO,BIT_FLASH_SPI_MISO);
//
//		delay_us(1);
//	}
//	GPIO_PinOutClear(GPIO_FLASH_SPI_CLK, BIT_FLASH_SPI_CLK);
//
//	return tr_data;          		//�����յ�������
//}
//
//
//
//void SPI2_Init(void)
//{
////    CMU_ClockEnable(cmuClock_GPIO, true);
////    GPIO_PinModeSet(GPIO_FLASH_SPI_CS,   BIT_FLASH_SPI_CS, gpioModePushPull, 1);
////    GPIO_PinModeSet(GPIO_FLASH_SPI_CLK,  BIT_FLASH_SPI_CLK, gpioModePushPull, 1);
////    GPIO_PinModeSet(GPIO_FLASH_SPI_MOSI, BIT_FLASH_SPI_MOSI, gpioModePushPull, 1);
////    GPIO_PinModeSet(GPIO_FLASH_SPI_MISO, BIT_FLASH_SPI_MISO, gpioModeInput, 1);
//
//    hFlashSpiPin = PIN_open(&pinFlashSpiState, FlashSpiPinTable);
//    if (NULL == hFlashSpiPin)
//    {
//        TRACE_ERROR("hFlashSpiPin ��ʧ�ܣ���������\n");
//    }
//    else
//    {
//        TRACE_CODE("hFlashSpiPin �򿪳ɹ���\n");
//    }
//
//    GPIO_PinOutClear(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);
//    SPI2_ReadWriteByte(0xff);//��������
//    GPIO_PinOutSet(GPIO_FLASH_SPI_CS, BIT_FLASH_SPI_CS);
//}
//
