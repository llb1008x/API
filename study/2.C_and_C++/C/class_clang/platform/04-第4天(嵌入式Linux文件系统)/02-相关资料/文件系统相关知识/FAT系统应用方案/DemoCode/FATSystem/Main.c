//============================================================
// 工程名称：FATSystem.spj
// 适应芯片: SPCE061A
// 实现功能: 演示FAT文件系统基本功能
//
// 涉及的库：(a) CMacro.lib
//             (b) clib.lib
//
// 组成文件:
//		Main.c, IRQ.c, SPCE061A.h, SPCE061.inc
//		FAT\FAT_Driver.c, FAT\FAT_Cache.c, FAT\FAT_Clus.c, FAT\FAT_Dir.c,
//		FAT\FAT_Disk.c, FAT\FAT_FDT.c, FAT\FAT_File.c, FAT\FAT.h
//		Key\Key.c, Key\Key.h
//		SD\SD_Command.c, SD\SD_CRC.c, SD\SD_Driver.c, SD\SD_Hardware.c, SD\SD_CFG.h
//		SD\SD_Command.h, SD\SD_CRC.h, SD\SD_Driver.h, SD\SD_Hardware.h
//		SPLC501\SPLC501User.c, SPLC501\SPLC501Driver_IO.asm, SPLC501\DataOSforLCD.asm,
//		SPLC501\SPLC501User.h, SPLC501\SPLC501Driver_IO.inc
//
// 维护记录：	2006-8-03	v1.0
//===============================================

///////////////////////////////////////////////////////////////////////////////
// 文件名称： Main.c
// 描述:  演示FAT文件系统基本功能
//		LCD  D0~D7接IOA8~IOA15
//		LCD  EP   接IOB14
//		LCD  RWP  接IOB13
//		LCD  A0   接IOB12
//		LCD  /CS1 接IOB15	或接地也可
//
//		SD_BUS_CS			IOB0
//		SD_BUS_SCK			IOB2
//		SD_BUS_Dout			IOB1
//		SD_BUS_Din			IOB3
//
//		IO高电平：3.3V     
//		IOA0~7接八个按键（按键公共端接低电平）
//		Key1	IOA0	读read.txt文件按键
//		Key2	IOA1	创建write.c文件按键
//		Key3	IOA2	追加unsp.doc文件按键
//		key4	IOA3	删除read.txt文件按键
//		key5	IOA4	建立text目录按键
//		key6	IOA5	删除yhma目录按键
//		key7	IOA6	删除驱动按键
//		key8	IOA7	清屏按键
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
	LCD501_Init(0x00);						// SPLC501初始化
 	LCD501_SetPaintMode(PAINT_COVER);
	LCD501_FontSet(0);
	DrvKeyInit();							// 键盘扫描初始化
	*P_INT_Ctrl= C_IRQ5_2Hz | C_IRQ4_1KHz;
 	 __asm("INT IRQ");
	
	FileInit();
	// 增加ATA设备0 
	AddFileDriver(SDCammand);
	
	while(1)
	{
		if(gui_KeyState != C_NullKeyState)	// 如有键按下则响应
		{
			switch(gui_KeyID)
			{
				case File_Read:		   		// 只读方式读取文件数据
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
    			case File_Write:			// 只写方式写文件数据
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
				case File_RW:				// 读写方式读写文件数据                                                        
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
    			case File_Remove:			// 删除文件
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
    			case Make_Dir:				// 建立目录   
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
    			case Remove_Dir:			// 删除目录
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
    			case Remove_Driver:			// 删除驱动
    				LCD501_PutString(0,56,(unsigned int *)"7.Dle Disk!");
    				RemoveFileDriver(0);
    				break;
    			case ClrScreen:
    				LCD501_ClrScreen(0);	// 清屏
    				break;
    			default:
    				break;
			}
			gui_KeyState = C_NullKeyState;
	   	}
	}
}
