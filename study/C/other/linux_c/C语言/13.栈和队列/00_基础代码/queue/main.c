#include <stdio.h>
#include "queue.h"

int main()
{
	int i = 0;
	while (enqueue(i++));

	while((i=dequeue())!=-1)
		printf("%d\n",i);

	return 0;
}