/*
	�ն˴�ӡ�����ӳ��򣬻�ȡ���λ�ã�������Ƶ���ָ��λ��
*/

#include "windows.h"

//��ȡ��ǰ���λ�ã�����ŵ����� x,y��
void whereXY(int *x,int *y) 
{ 
	HANDLE h1; 
	CONSOLE_SCREEN_BUFFER_INFO scrInfo; 
	h1=GetStdHandle(STD_OUTPUT_HANDLE); 	
	GetConsoleScreenBufferInfo(h1,&scrInfo); 	
	*x=scrInfo.dwCursorPosition.X; 
	*y=scrInfo.dwCursorPosition.Y; 
} 
//��ת�����ָ��λ��
void GoToXY(int x,int y) 
{ 
	HANDLE h1; 
	COORD pos;
	pos.X=x;
	pos.Y=y;
	h1=GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(h1,pos); 
} 
//���ý������ն���ʾ�ı��ı���ɫ���ı���ɫ
void SetText_Color(int color) 
{ 
	HANDLE hStdout;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,color);
} 