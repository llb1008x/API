#include <stdio.h>

int main()
{
	int a[3];
	int i = 0;

	while (scanf("%d%*c", a + i++) == 1)
					;

	for(i=0;i<3;i++)
	{
		printf("a[%d]->%d\n",i,a[i]);
	}	

	return 0;

}
