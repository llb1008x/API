/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月08日 星期三 20时26分51秒
 ************************************************************************/

#include <stdio.h>

int main()
{
	printf("hello world!\n");

	double fahr,celsius;
	double lower,upper,step;
	lower=0;
	upper=300;
	step=20;
	
	fahr=lower;
	while(fahr<=upper){
	
		celsius=5.0*(fahr-32)/9.0;
		printf("%3f\t%6f\n",fahr,celsius);
		fahr += step;
	}

  
    return 0;
}
