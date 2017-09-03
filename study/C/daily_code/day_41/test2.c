#include <stdio.h>

void fun()
{
	static int f=1;
	int w=1;
	
	w += 2;
	f += 2;
	
	printf("w->%d,f->%d\n",w,f);
}

int main()
{
	int i;
	for(i=0;i<5;i++){
	
		fun();
	}	
	
	return 0;
}
