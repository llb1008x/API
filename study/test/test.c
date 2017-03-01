#include <stdio.h>
#include <time.h>
#include <string.h>

clock_t start,stop;

double duration;

int main(int argc,char *argv[])
{

	start=clock();

	int i=atoi(argv[1]);
	while(i)
		i--;	
					;
	stop=clock();

	duration=(double)(stop-start)/CLOCKS_PER_SEC;

	printf("duration--->%lf\n",duration);

	return 0;
}
