#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add(int a , int b)
{
	return a+b;
}

int sub(int a , int b)
{
	return a-b;
}

int mux(int a , int b)
{
	return a*b;
}

int dive(int a , int b)
{
	return a/b;
}

int mod(int a, int b)
{
	return a%b;
}

int process(int a,int b, int (*fun)(int a,int b) )
{
	int result;
	result=(*fun)(a,b);
	return result;
}

char *fun_str[5]={"add","sub","mux","div","mod"};			//指针数组
int (*fun_name[5])(int ,int) = {add,sub,mux,dive,mod};	//函数指针数组

int main()
{
	int a = 0;
	int b = 0;
	int result = 0;
	char buf[20] = "";
	char *input[4];
	char *cmd;
	int i = 0;
	while (1)
	{
		input[0]=NULL;
		input[1]=NULL;
		input[2]=NULL;
		i=0;

		printf("please inpt cmd:");
		gets(buf);
		
		input[i]=strtok(buf," ");
		while(input[i]!=NULL)
		{
			input[++i]=strtok(NULL," ");
		}
		cmd=input[0];
	
		if(input[1]==NULL||input[2]==NULL)
		{
			printf("你输入的参数不够\n");
			continue;
		}
	
		a = atoi(input[1]);
		b = atoi(input[2]);
	
		for(i=0;i<5;i++)
		{
			if( strcmp(cmd,fun_str[i])==0 )
			{
				result=process(a,b,fun_name[i]);
				printf("result = %d\n", result);
				break;
			}
		}
	}
	return 0;
}
