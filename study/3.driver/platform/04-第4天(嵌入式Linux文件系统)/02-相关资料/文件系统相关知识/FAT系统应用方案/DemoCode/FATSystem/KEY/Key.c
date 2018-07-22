//======================================================
// �ļ����ƣ�	Key.c
// ����������	����ɨ��
// ά����¼��	2006-8-03	v1.0
//======================================================
#include "Key.h"

unsigned int gui_KeyState;						//	��������
unsigned int gui_KeyID;							//	����ID
unsigned int gui_KeyScanState;					//	����ɨ��״̬
unsigned int gui_KeyScanID;						//	����ɨ��ID
unsigned int gui_KeyCount;						//	Key Scan������
//=============================================================
// �﷨��ʽ��	void DrvKeyInit(void)
// ʵ�ֹ��ܣ�	������ʼ��
// ������		��
// ����ֵ��		��
//=============================================================
void DrvKeyInit()
{
	gui_KeyState = C_NullKeyState;
	gui_KeyID = C_NullKey;
	gui_KeyScanState = C_NullKeyState;
	gui_KeyScanID = C_NullKey;
	gui_KeyCount = 0;
	*P_KEY_Dir &= ~KEY_PORT_MASK;
	*P_KEY_Attrib &= ~KEY_PORT_MASK;
#if KEY_COM
	*P_KEY_Buffer &= ~KEY_PORT_MASK;
#else
	*P_KEY_Buffer |= KEY_PORT_MASK;
#endif
}
//=============================================================
// �﷨��ʽ��	unsigned int DrvGetKeyID(void)
// ʵ�ֹ��ܣ�	ȡ�ð���ID
// ������		��
// ����ֵ��		����ID
//=============================================================
unsigned int DrvGetKeyID()
{
	unsigned int uiRet;
	uiRet = *P_KEY_Data & KEY_PORT_MASK;
#if !KEY_COM
	uiRet ^= KEY_PORT_MASK;
#endif
	return(uiRet);
}
//=============================================================
// �﷨��ʽ��	void KeyScanServiceLoop(void)
// ʵ�ֹ��ܣ�	������̨ɨ��
// ������		��
// ����ֵ��		��
//=============================================================
void KeyScanServiceLoop()
{
	gui_KeyScanID = DrvGetKeyID();
	if(gui_KeyScanID != C_NullKey)
	{											//	�м�����
		gui_KeyCount++;
		gui_KeyState = C_NullKeyState;
		if(gui_KeyCount > C_LKeyCount)
		{										//	������
			if(gui_KeyID == gui_KeyScanID)
			{
				gui_KeyScanState = C_LKeyState;
			}
			else
			{									//	��Ч����
				gui_KeyID = gui_KeyScanID;
				gui_KeyScanState = C_NullKeyState;
			}
		}
		else if(gui_KeyCount > C_SKeyCount)
		{										//	�̰���
			if(gui_KeyID == gui_KeyScanID)
			{
				gui_KeyScanState = C_SKeyState;
			}
			else
			{									//	��Ч����
				gui_KeyID = gui_KeyScanID;
				gui_KeyScanState = C_NullKeyState;
			}
		}
		else
		{										//	ȥ��
			if(gui_KeyID != gui_KeyScanID)
			{									//	��Ч����
				gui_KeyID = gui_KeyScanID;
				gui_KeyScanState = C_NullKeyState;
			}
		}

	}
	else if(gui_KeyScanState != C_NullKeyState)
	{											//	����̧��
		gui_KeyState = gui_KeyScanState;
		gui_KeyScanState = C_NullKeyState;
		gui_KeyCount = 0;
	}
}