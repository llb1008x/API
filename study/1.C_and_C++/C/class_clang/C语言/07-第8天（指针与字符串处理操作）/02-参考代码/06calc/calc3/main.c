#include "sub.h"

typedef float (p_fun)(float,float);
int main(void)
{
	int i=0,n=0;
	float reslut;

	char *input[10]={NULL};
	char *name[]={"add","sub","mux","dive"};
	p_fun *name_fun[] ={add,sub,mux,dive};
	float num[2];
	char buf[10];
	int loop=1;
	while(loop)
	{
		i=0;
		printf("please input like this:(add 1 2)");
		gets(buf);
		input[i]=strtok(buf," ");
		while(input[i]!=NULL)
		{
			input[++i]=strtok(NULL," ");
		}

		if(input[0]==NULL)
			continue;

		if( strcmp(input[0],"exit")==0)
		{
			loop=exit1();
			continue;
		}
		else if(strcmp(input[0],"help")==0)
		{
			help();
			continue;
		}
		else if(strcmp(input[0],"cls")==0)
		{
				system("cls");
				continue;
		}

		if(input[1]==NULL||input[2]==NULL)
		{
			printf("你输入的参数不够\n");
			input[0]=NULL;
			input[1]=NULL;
			input[2]=NULL;
			continue;
		}
		num[0]=(float)atof(input[1]);
		num[1]=(float)atof(input[2]);

		for(i=0;i<4;i++)
		{
			if( strcmp(name[i],input[0])==0)
			{
				reslut=fun(num[0],num[1],name_fun[i]);
				printf("%3.4f\n",reslut);
				break;
			}
		}
		if(i==4)
		{
			printf("您所输入的命令不存在\n");
			help();
			continue;
		}
		input[0]=NULL;
		input[1]=NULL;
		input[2]=NULL;
	}
}

