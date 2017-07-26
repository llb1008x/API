/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年07月26日 星期三 19时46分48秒
	打字游戏
	
	1.产生随机数
	
	2.无回显输入数字，但其实这里应该是输入字符
	
	3.字符匹配
	
	4.用时
	
 ************************************************************************/

#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

#define MAX_SIZE		5

//无阻塞，无回显的getch
char mygetch()
{	
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

int main()
{
	//1.产生一组随机数，然后存在数组中
	int i=0,count=0;
	double score;
	char dst[MAX_SIZE]="",str[MAX_SIZE]="";
	int start_time,edn_time,period;
	
	system("clear");
	help();
	srand((unsigned )time(NULL)); 
	
	start_time=time(NULL);
	for(i=0;i<MAX_SIZE;i++){
	
		dst[i]=rand()%26+'a';
		printf("%c  ",dst[i]);
	}
	printf("\n");

	//3.获取键值,获取自己输入的，存在数组中，然后与随即的进行对比
	char ch;
	i=0;
	while( ((ch=mygetch())!='_')&&(i<MAX_SIZE)){
		
		printf("%c",ch);
		fflush(NULL);
		str[i]=ch;
		//printf("str[%d]->%c\n",i,str[i]);
		i++;
	}
	
	//4.记录时间，用于计时,总共用了多长时间
	edn_time=time(NULL);
	period=edn_time-start_time;
	printf("period->%d\n",period);
	
	//5.计算正确率
	for(i=0;i<MAX_SIZE;i++){
		
		if(dst[i]==str[i]){
			
			count++;
		}
	}
	
	score = 1.0*count/MAX_SIZE;
	printf("count->%d,score->%.2lf%%\n",count,score*100);

	return 0;
}
