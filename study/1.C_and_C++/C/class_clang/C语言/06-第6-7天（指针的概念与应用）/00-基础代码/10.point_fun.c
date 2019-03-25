#include <stdio.h>
//����ָ��
extern int max (int,int);					//�������� 
extern int min (int,int);					//�������� 
extern int add (int,int);     				//��������	
extern int process ( int,int,int(*fun)(int,int) );//��������
	
int main ()
{
	int a,b,result;
	
	printf ("enter a and b:");
	scanf("%d %d",&a,&b);

	printf("max=");
	result=process(a,b,max);
	printf("%d\n",result);

	printf("min=");
	result=process(a,b,min);
	printf("%d\n",result);
	
	printf("sum=");
	result=process(a,b,add);
	printf("%d\n",result);
	return 0;
}

int max( int x, int y)
{
	return x>y?x:y;
}

int min( int x ,int y)
{
	return x<y?x:y;
}

int add(int x, int y )
{
	return x+y;
}

int  process(int x,int y,int (*fun)(int,int))
{
	int result;
	result = (*fun)(x,y);
	return result;
}

