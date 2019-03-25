/******************************************************
					计算阶乘
******************************************************/
#include <stdio.h>
#include <string.h>

#define FUNCTION

#if defined(FUNCTION)

int factial(int num)
{
	if(num>1){

		return factial(num-1)*num;
	}else if(num==1){
		
		return 1;
	}else {

		printf("error\n");
		return -1;
	}

}



#else
int factial(int num)
{
	int i,t=1;

	for(i=1;i<=num;i++){


 		t *= i;
	}
	
	return t;
}
#endif


int main(int argc,char *argv[])
{
	int t;
	
	if( argc!=2){

		printf("error\n");
		return -1;
	}
		
	t=factial(atoi(argv[1]));

	printf("t->%d\n",t);

	return 0;
}
