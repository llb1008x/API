/*
	终端打印控制子程序，获取光标位置，将光标移到到指定位置
*/

#include "windows.h"

//获取当前光标位置，并存放到变量 x,y中
void whereXY(int *x,int *y) 
{ 
	HANDLE h1; 
	CONSOLE_SCREEN_BUFFER_INFO scrInfo; 
	h1=GetStdHandle(STD_OUTPUT_HANDLE); 	
	GetConsoleScreenBufferInfo(h1,&scrInfo); 	
	*x=scrInfo.dwCursorPosition.X; 
	*y=scrInfo.dwCursorPosition.Y; 
} 
//跳转到光标指定位置
void GoToXY(int x,int y) 
{ 
	HANDLE h1; 
	COORD pos;
	pos.X=x;
	pos.Y=y;
	h1=GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(h1,pos); 
} 
//设置接下来终端显示文本的背景色和文本颜色
void SetText_Color(int color) 
{ 
	HANDLE hStdout;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,color);
} 