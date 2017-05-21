//简单的字母输入测试程式
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#define AL  51 //定义测试字母的最大长度
char mygetch( ) {
    struct termios oldt, newt;
    char ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}
void help()
{
	printf("\n*****************************************");
	printf("\n* 输入过程中无法退出!                   *");
	printf("\n* 请按所给字母敲击键盘!                 *");
	printf("\n* 按任意键开始测试,按下首字母时开始计时!*");
	printf("\n* 输入出错则以 _ 表示                   *");
	printf("\n*****************************************\n\n");
}

main()
{
	char str[AL] = "";

	char ch;
	int i=0;
	int count=0;
	int k;
	time_t t_start,t_end;

	while(1)
	{
		system("clear");//vc++下调用system();函数实现清屏
		srand((unsigned)time(NULL));//采集当前时间做为随机数rand()的开始
		
		for(k = 0;k<AL-1;k++)//打印50个英文字符用于测试
		{
			str[k] = rand() % 26 +'a' ;
		}
		
		str[AL-1] = '\0';//写完字符后补上结束标志位
		
		help();
	
		mygetch();
		
		printf("\n%s\n",str);
		count =0;
		for(i=0;i<AL-1;i++)
		{
			ch = mygetch();
			if(i==0)
				t_start = time(NULL);//取开始时间
			if(ch == str[i])
			{
				printf("%c",ch);
				count++;
			}
			else
			{
				printf("_");//打印出错标志
			}
		}
		t_end = time(NULL);//取结束时间
		printf("\n完成输入!\n用时 %d s\n正确率%lf %c\n按下Esc退出，任意键继续!\n\n",(t_end-t_start),count*100.0/50,'%');
		while(1)
		{
			ch=mygetch();
			if(ch == 27) 
			return 0;//若为esc再次退出，以退出程序
			if(ch==' ')
			break;
		}
	}
}