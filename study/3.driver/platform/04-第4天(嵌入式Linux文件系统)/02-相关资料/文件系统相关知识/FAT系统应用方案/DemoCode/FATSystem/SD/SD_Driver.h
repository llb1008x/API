//======================================================
// �ļ����ƣ�	SD_Driver.c
// ����������	SD�������ͷ�ļ�
// ά����¼��	2006-8-03	v1.0
//======================================================
#ifndef __SDDRIVER_H__
#define __SDDRIVER_H__

typedef struct _SDINFO
{	
	unsigned long int BlockNum;			//	����Block������
	unsigned long int BlockLen;			//	Block��С
	unsigned long int EraseNum;			//	һ�οɲ�����Block����

	unsigned long int rTimOut;			//	��Block��ʱʱ��
	unsigned long int wTimOut;			//	д�鳬ʱʱ��
	unsigned long int eTimOut;			//	���鳬ʱʱ��
}SDINFO;

extern SDINFO SDInfo;						//	SD����Ϣ

extern unsigned char SD_Initialize(void);

extern unsigned char SD_ReadBlock(unsigned long int BlockAddr, unsigned char *BlockBuf);

extern unsigned char SD_WriteBlock(unsigned long int BlockAddr, unsigned char *BlockBuf);

extern unsigned char SD_EraseBlock(unsigned long int BlockStartAddr, unsigned long int BlockNum);

extern unsigned char SD_GetCardInfo();

extern void SD_CalTimeout(unsigned char *CSDBuf);

extern unsigned char SD_ActiveInit(void);

//	������
#define   SD_NO_ERR			     	0x00			//	����ִ�гɹ�
#define   SD_ERR_NO_CARD		 	0x01			//	SD��û����ȫ���뵽������
#define   SD_ERR_USER_PARAM      	0x02			//	�û�ʹ��API����ʱ����ڲ����д���
#define   SD_ERR_CARD_PARAM		 	0x03			//	���в����д����뱾ģ�鲻���ݣ�
#define	  SD_ERR_VOL_NOTSUSP        0x04			//	����֧��3.3V����
#define   SD_ERR_OVER_CARDRANGE		0x05			//	����������������Χ

//	Command���ܷ��صĴ�����
#define   SD_ERR_CMD_RESPTYPE	 	0x10			//	�������ʹ���
#define   SD_ERR_CMD_TIMEOUT     	0x11			//	SD������Ӧ��ʱ
#define   SD_ERR_CMD_RESP		 	0x12			//	SD������Ӧ����
			
//	Data������
#define   SD_ERR_DATA_CRC16      	0x20			//	������CRC16У�鲻ͨ��
#define   SD_ERR_DATA_START_TOK		0x21			//	���������ʱ�����ݿ�ʼ���Ʋ���ȷ
#define	  SD_ERR_DATA_RESP		 	0x22			//	д�������ʱ��SD��������Ӧ���Ʋ���ȷ

//	TimeOut������
#define   SD_ERR_TIMEOUT_WAIT    	0x30			//	д�������ʱ��������ʱ����
#define   SD_ERR_TIMEOUT_READ    	0x31			//	��������ʱ����
#define	  SD_ERR_TIMEOUT_WRITE	 	0x32			//	д������ʱ����
#define   SD_ERR_TIMEOUT_ERASE   	0x33			//	����������ʱ����
#define	  SD_ERR_TIMEOUT_WAITIDLE 	0x34			//	��ʼ��SD��ʱ���ȴ�SD���������״̬��ʱ����

//	Write Operate���ܷ��صĴ�����
#define	  SD_ERR_WRITE_BLK			0x40			//	д�����ݴ���
#define	  SD_ERR_WRITE_BLKNUMS      0x41			//	д���ʱ����Ҫд��Ŀ�����ȷд��Ŀ�����һ��
#define   SD_ERR_WRITE_PROTECT		0x42			//	����ǵ�д�������ش���д����λ��

#define SD_INIT_TIMEOUT			1000
#define SD_CMD_TIMEOUT			100
#define SD_READ_TIMEOUT			100 * SPI_CLOCK / 1000 / 8
#define SD_WRITE_TIMEOUT		250 * SPI_CLOCK / 1000 / 8    
#endif