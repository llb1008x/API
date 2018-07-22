//============================================================
// �������ƣ�FATSystem.spj
// ��ӦоƬ: SPCE061A
// ʵ�ֹ���: ��ʾFAT�ļ�ϵͳ��������
//
// �漰�Ŀ⣺(a) CMacro.lib
//             (b) clib.lib
//
// ����ļ�:
//		Main.c, IRQ.c, SPCE061A.h, SPCE061.inc
//		FAT\FAT_Driver.c, FAT\FAT_Cache.c, FAT\FAT_Clus.c, FAT\FAT_Dir.c,
//		FAT\FAT_Disk.c, FAT\FAT_FDT.c, FAT\FAT_File.c, FAT\FAT.h
//		Key\Key.c, Key\Key.h
//		SD\SD_Command.c, SD\SD_CRC.c, SD\SD_Driver.c, SD\SD_Hardware.c, SD\SD_CFG.h
//		SD\SD_Command.h, SD\SD_CRC.h, SD\SD_Driver.h, SD\SD_Hardware.h
//		SPLC501\SPLC501User.c, SPLC501\SPLC501Driver_IO.asm, SPLC501\DataOSforLCD.asm,
//		SPLC501\SPLC501User.h, SPLC501\SPLC501Driver_IO.inc
//
// ά����¼��	2006-8-03	v1.0
//===============================================

///////////////////////////////////////////////////////////////////////////////
// �ļ����ƣ� Main.c
// ����:  ��ʾFAT�ļ�ϵͳ��������
//		LCD  D0~D7��IOA8~IOA15
//		LCD  EP   ��IOB14
//		LCD  RWP  ��IOB13
//		LCD  A0   ��IOB12
//		LCD  /CS1 ��IOB15	��ӵ�Ҳ��
//
//		SD_BUS_CS			IOB0
//		SD_BUS_SCK			IOB2
//		SD_BUS_Dout			IOB1
//		SD_BUS_Din			IOB3
//
//		IO�ߵ�ƽ��3.3V     
//		IOA0~7�Ӱ˸����������������˽ӵ͵�ƽ��
//		Key1	IOA0	��read.txt�ļ�����
//		Key2	IOA1	����write.c�ļ�����
//		Key3	IOA2	׷��unsp.doc�ļ�����
//		key4	IOA3	ɾ��read.txt�ļ�����
//		key5	IOA4	����textĿ¼����
//		key6	IOA5	ɾ��yhmaĿ¼����
//		key7	IOA6	ɾ����������
//		key8	IOA7	��������
//========================================================================================

#include "FAT\FAT.h" 
#include "String.h"
#include "SD\SD_INC.h"
#include "SPLC501\SPLC501User.h"
#include "Key\Key.h"

extern unsigned int SDCammand(unsigned char Cammand, void *Parameter);

int  main (void)
{ 
	unsigned int uiTemp;
	unsigned char Ret;
	HANDLE FHandle;
	char string[15] ;
	unsigned char FileBuf[15] ;
	char WritFileData[15] ;
	LCD501_Init(0x00);						// SPLC501��ʼ��
 	LCD501_SetPaintMode(PAINT_COVER);
	LCD501_FontSet(0);
	DrvKeyInit();							// ����ɨ���ʼ��
	*P_INT_Ctrl= C_IRQ5_2Hz | C_IRQ4_1KHz;
 	 __asm("INT IRQ");
	
	FileInit();
	// ����ATA�豸0 
	AddFileDriver(SDCammand);
	
	while(1)
	{
		if(gui_KeyState != C_NullKeyState)	// ���м���������Ӧ
		{
			switch(gui_KeyID)
			{
				case File_Read:		   		// ֻ����ʽ��ȡ�ļ�����
					LCD501_PutString(0,0,(unsigned int *)"1.R read.txt!");                    
					strcpy(string,"a:\\read.txt");
					FHandle = FileOpen(string, "R");
					if(Not_Open_FILE != FHandle) 
					{
						uiTemp = FileRead(FileBuf,50, FHandle);
    					FileBuf[uiTemp] = 0;
    					LCD501_PutString(8,8,(unsigned int *)FileBuf);
    					LCD501_PutString(106,8,(unsigned int *)"OK!");
					}
					else
					{
    					LCD501_PutString(94,8,(unsigned int *)"FAIL!");
					}
					FileClose(FHandle);
    				break;
    			case File_Write:			// ֻд��ʽд�ļ�����
    				LCD501_PutString(0,16,(unsigned int *)"2.W write.c!");
					strcpy(string,"a:\\write.c");
					strcpy(WritFileData,"Creat Write.c\n");
					FHandle = FileOpen(string, "W");                     
					uiTemp = FileWrite(WritFileData, strlen(WritFileData), FHandle);
					FileClose(FHandle);
					if(uiTemp != 0) 
					{
    					LCD501_PutString(106,16,(unsigned int *)"OK!");
					}
					else
					{
    					LCD501_PutString(94,16,(unsigned int *)"Fail!");
					}
    				break;
				case File_RW:				// ��д��ʽ��д�ļ�����                                                        
    				LCD501_PutString(0,24,(unsigned int *)"3.RW unsp.doc!");
					strcpy(string,"a:\\unsp.doc");
					strcpy(WritFileData,"RW unsp.doc\n");
					FHandle = FileOpen(string, "RW");
					if(Not_Open_FILE != FHandle) 
					{
    					LCD501_PutString(106,24,(unsigned int *)"OK!");
					}
					else
					{
    					LCD501_PutString(94,24,(unsigned int *)"Fail!");
					}
					FileSeek(FHandle,0, SEEK_END);
					FileWrite(WritFileData, strlen(WritFileData), FHandle);
					FileClose(FHandle);
    				break;
    			case File_Remove:			// ɾ���ļ�
    				LCD501_PutString(0,32,(unsigned int *)"4.Dle read.txt!");
					strcpy(string,"a:\\read.txt");
					Ret = RemoveFile(string);
					if(Ret== RETURN_OK)
					{
						LCD501_PutString(106,32,(unsigned int *)"OK!");
					}
					else
					{
						LCD501_PutString(94,32,(unsigned int *)"Fail!");
					}
    				break;
    			case Make_Dir:				// ����Ŀ¼   
    				LCD501_PutString(0,40,(unsigned int *)"5.Make test!");
					strcpy(string,"A:\\test");
					Ret = MakeDir(string);
					if(Ret == RETURN_OK)
					{
						LCD501_PutString(106,40,(unsigned int *)"OK!");
					}
					else
					{
						LCD501_PutString(94,40,(unsigned int *)"Fail!");
					}
    				break;
    			case Remove_Dir:			// ɾ��Ŀ¼
    				LCD501_PutString(0,48,(unsigned int *)"6.Dle yhma!");
					strcpy(string,"A:\\yhma");
					Ret = RemoveDir(string);
					if(Ret == RETURN_OK)
					{
						LCD501_PutString(106,48,(unsigned int *)"OK!");
					}
					else
					{
						LCD501_PutString(94,48,(unsigned int *)"Fail!");
					}
    				break;    				
    			case Remove_Driver:			// ɾ������
    				LCD501_PutString(0,56,(unsigned int *)"7.Dle Disk!");
    				RemoveFileDriver(0);
    				break;
    			case ClrScreen:
    				LCD501_ClrScreen(0);	// ����
    				break;
    			default:
    				break;
			}
			gui_KeyState = C_NullKeyState;
	   	}
	}
}
