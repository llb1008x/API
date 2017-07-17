
#include <stdio.h>
#include "add.h"
#include "sub.h"
#include "mux.h"
#include "div.h"

int main(void)
{
	int a = 20;
	int b = 10;
	
	printf("a = %d\n", a);
	printf("b = %d\n", b);
	printf("a + b = %d\n", add(a,b));	//求a b之和
	printf("a - b = %d\n", sub(a,b));	//求a b之差
	printf("a * b = %d\n", mux(a,b));   //求a b之积
	printf("a / b = %d\n", div(a,b));   //求a b之商
	return 0;
}


