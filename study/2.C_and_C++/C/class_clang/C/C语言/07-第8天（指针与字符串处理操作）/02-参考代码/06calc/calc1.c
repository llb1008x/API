#include <stdio.h>
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

int main()
{
	int (*fun)(int a, int b);
	char cmd[5] = "";
	int a = 0;
	int b = 0;
	int result = 0;
	while (1)
	{
		printf("please inpt cmd:");
		scanf("%s %d %d", cmd, &a, &b);
		if(strcmp(cmd, "add")==0)
			result=add(a,b);
		else if(strcmp(cmd, "sub")==0)
			result=sub(a,b);
		else if(strcmp(cmd, "mux")==0)
			result=mux(mux,a,b);
		else if(strcmp(cmd, "div")==0)
			result=div(a,b);
		else if(strcmp(cmd, "mod")==0)
			result=mod(a,b);
		else
		{
			printf("unknow cmd \" %s \"\n", cmd);
			continue ;
		}
		printf("result = %d\n", result);
	}
	return 0;
}
