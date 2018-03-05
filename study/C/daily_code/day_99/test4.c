/*************************************************************************
	> File Name: test4.c
	> Author: 
	> Mail: 
	> Created Time: 2018年03月05日 星期一 20时27分56秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main()
{
#if 0
	char *ptr;
	if((ptr=(char *)malloc(0))==NULL){
		
		printf("got a null pointer\n");
	}else {
		
		printf("got a valid pointer\n");
	}
#endif 
	int a=5,b=7,c;
	c=a+++b;
	
	printf("a->%d,b->%d,c->%d\n",a,b,c);	
	

    return 0;
}
