#include<stdio.h>
#include<stdlib.h>
int main()
{
	int i;
	
	int *pn=(int *)malloc(5*sizeof(int));
	
	for(i=0;i<5;i++)
		scanf("%d",&pn[i]);
	
	pn=(int *)realloc(pn,10*sizeof(int));
	
	for(i=5;i<10;i++)
		scanf("%d",&pn[i]);
	
	for(i=0;i<10;i++)
		printf("%d ",pn[i]);
	
	free(pn);
	return 0;
}