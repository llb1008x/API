#include "sub.h"
int exit1(void)
{
		char ch;
		int flag=0;
		printf("ȷ���˳�? Y? N?");
		ch=getchar();
		if(ch==' Y' || ch=='y')
			flag=0;
		else if (ch==' N' || ch=='n')
			flag=1;
		return flag; 
}
void help()
{
	system("cls");
	printf("��ǰ������֧����������:");
	printf("add,sub,div,mux\n");
	printf("��ʽ:add ����1 ����2\n");
}

float add(float a,float b)
{
	return (a+b);
}

float sub(float a,float b)
{
	return (a-b);
}

float mux(float a,float b)
{
	return (a*b);
}

float dive(float a,float b)
{
	return (a/b);
}

float fun(float a,float b,float(*fun)(float a,float b))
{
	float m;
	m=(*fun)(a,b);
	return m;
}