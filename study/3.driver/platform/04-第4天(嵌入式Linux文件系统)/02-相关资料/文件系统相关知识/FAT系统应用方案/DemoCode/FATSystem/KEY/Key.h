#ifndef	__KEY_h__
#define	__KEY_h__
//	write your header here
#include "SPCE061A.h"

//	�Զ��尴��
#define C_NullKey	  0x0000
#define File_Read     0x0001
#define File_Write    0x0002
#define File_RW       0x0004
#define File_Remove   0x0008
#define Make_Dir      0x0010
#define Remove_Dir    0x0020
#define Remove_Driver 0x0040
#define ClrScreen     0x0080

//	�����Զ��尴��

//*************************************
//	�����������
#define PORT_A 1
#define PORT_B 0
#define COM_LOW 0
#define COM_HIGH 1
//	�����������
//*************************************


//	Hardware definition
#define KEY_PORT PORT_A		//PORT_B	//	����IO�ڶ���
#define KEY_PORT_MASK 0x00ff			//	redefine this to fit for your system
#define KEY_COM COM_LOW	//COM_HIGH	//	������
//	End of Hardware definition





//*************************************
//	�����������

extern unsigned int gui_KeyState;		//	��������
extern unsigned int gui_KeyID;			//	��ֵ

extern void DrvKeyInit();
extern void KeyScanServiceLoop();

#define C_NullKeyState	0x0000
#define C_SKeyState		0x0001
#define C_LKeyState		0x0002

#define C_SKeyCount	13					//	ȥ��ʱ�䣨���͵�λֵ��ms��
#define C_LKeyCount	2048				//	������ʱ�䣨���͵�λֵ��ms��


#if KEY_PORT
	#define P_KEY_Dir P_IOA_Dir
	#define P_KEY_Attrib P_IOA_Attrib
	#define P_KEY_Data P_IOA_Data
	#define P_KEY_Buffer P_IOA_Buffer
#else
	#define P_KEY_Dir P_IOB_Dir
	#define P_KEY_Attrib P_IOB_Attrib
	#define P_KEY_Data P_IOB_Data
	#define P_KEY_Buffer P_IOB_Buffer
#endif
//	�����������
//*************************************

#endif
