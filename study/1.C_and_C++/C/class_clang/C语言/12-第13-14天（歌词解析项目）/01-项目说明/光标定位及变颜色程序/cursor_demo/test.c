#include<stdio.h>
#include"console.h"
int main()
{
	clear_screen();//清屏幕内容
	cusor_moveto(20, 4);//光标移到 第4行，第20列
	set_fg_color(COLOR_RED);//设置字体颜色为红色
	printf("hello ,i love China!!!\n");
	
	cusor_moveto(20, 2);//光标移到 第2行，第20列
	set_fg_color(COLOR_BLUE);//设置字体颜色为蓝色
	printf("hello ,i love sunplusapp!!\n");
	return 0;
}
