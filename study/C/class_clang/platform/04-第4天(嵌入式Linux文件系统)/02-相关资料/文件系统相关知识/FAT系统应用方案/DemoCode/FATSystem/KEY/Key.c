//======================================================
// 文件名称：	Key.c
// 功能描述：	按键扫描
// 维护记录：	2006-8-03	v1.0
//======================================================
#include "Key.h"

unsigned int gui_KeyState;						//	按键类型
unsigned int gui_KeyID;							//	按键ID
unsigned int gui_KeyScanState;					//	按键扫描状态
unsigned int gui_KeyScanID;						//	按键扫描ID
unsigned int gui_KeyCount;						//	Key Scan计数器
//=============================================================
// 语法格式：	void DrvKeyInit(void)
// 实现功能：	按键初始化
// 参数：		无
// 返回值：		无
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
// 语法格式：	unsigned int DrvGetKeyID(void)
// 实现功能：	取得按键ID
// 参数：		无
// 返回值：		按键ID
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
// 语法格式：	void KeyScanServiceLoop(void)
// 实现功能：	按键后台扫描
// 参数：		无
// 返回值：		无
//=============================================================
void KeyScanServiceLoop()
{
	gui_KeyScanID = DrvGetKeyID();
	if(gui_KeyScanID != C_NullKey)
	{											//	有键按下
		gui_KeyCount++;
		gui_KeyState = C_NullKeyState;
		if(gui_KeyCount > C_LKeyCount)
		{										//	长按键
			if(gui_KeyID == gui_KeyScanID)
			{
				gui_KeyScanState = C_LKeyState;
			}
			else
			{									//	无效按键
				gui_KeyID = gui_KeyScanID;
				gui_KeyScanState = C_NullKeyState;
			}
		}
		else if(gui_KeyCount > C_SKeyCount)
		{										//	短按键
			if(gui_KeyID == gui_KeyScanID)
			{
				gui_KeyScanState = C_SKeyState;
			}
			else
			{									//	无效按键
				gui_KeyID = gui_KeyScanID;
				gui_KeyScanState = C_NullKeyState;
			}
		}
		else
		{										//	去抖
			if(gui_KeyID != gui_KeyScanID)
			{									//	无效按键
				gui_KeyID = gui_KeyScanID;
				gui_KeyScanState = C_NullKeyState;
			}
		}

	}
	else if(gui_KeyScanState != C_NullKeyState)
	{											//	按键抬起
		gui_KeyState = gui_KeyScanState;
		gui_KeyScanState = C_NullKeyState;
		gui_KeyCount = 0;
	}
}