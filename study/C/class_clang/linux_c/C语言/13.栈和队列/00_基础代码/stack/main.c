#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main()
{
	int i = 0;
	while(1)
	{
		int temp = 0;
		int mod = 0;
		printf("please input a num:");
		scanf("%d", &i);
		do 
		{	
			int mod = i%8;
			if(!full())
				push(mod);
			i = i/8;
		} while (i!=0);
		
		printf("OCT is:");
		while(!empty())
			printf("%d",pop());
		printf("\n");
	}
	return 0;
}