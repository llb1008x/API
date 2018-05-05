/****************************************************

*****************************************************/
#include <stdio.h>

int main()
{
	int a=2;
	int b=3;
	int array[3];
	
	array[0]=1;
	array[1]=10;
	array[2]=100;
	
	int *p=&a;
	int i;
	for(i=0;i<6;i++){
	
		printf("*p->%d\n",*p);
		p++;
	}
	printf("***************************************\n");
	p=&a;
	for(i=0;i<6;i++){
	
		printf("*p->%d\n",p[i]);
	}
	
//gdb  x/3d   	起始地址
	
	return 0;
}