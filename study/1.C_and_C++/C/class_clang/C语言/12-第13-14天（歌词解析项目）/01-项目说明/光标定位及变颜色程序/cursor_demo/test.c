#include<stdio.h>
#include"console.h"
int main()
{
	clear_screen();//����Ļ����
	cusor_moveto(20, 4);//����Ƶ� ��4�У���20��
	set_fg_color(COLOR_RED);//����������ɫΪ��ɫ
	printf("hello ,i love China!!!\n");
	
	cusor_moveto(20, 2);//����Ƶ� ��2�У���20��
	set_fg_color(COLOR_BLUE);//����������ɫΪ��ɫ
	printf("hello ,i love sunplusapp!!\n");
	return 0;
}
