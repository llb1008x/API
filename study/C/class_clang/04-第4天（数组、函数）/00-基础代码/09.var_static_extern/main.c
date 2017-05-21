#include <stdio.h>
extern int va;
extern int getG(void);
extern int getO(void);
int main(void)
{
	printf("va=%d\n",va);
	printf("getO=%d\n",getO());
	printf("getG=%d\n",getG());
	printf("%d", va*getO()*getG());
}