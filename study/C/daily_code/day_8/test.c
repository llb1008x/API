/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年04月30日 星期日 13时36分40秒
 ************************************************************************/

#include <stdio.h>

#define  BLURB "Authentic imitation"

int main()
{
	printf("hello world\n");
/*
	printf("/%2s/\n",BLURB);
	printf("/%24s/\n",BLURB);
	printf("/%24.5s/\n",BLURB);
	printf("/%-24.5s/\n",BLURB);
*/
	float n1=3.0;
	double n2=3.0;
	long n3=200000000;
	long n4=123456789;

	printf("%.1e %.1e %.1e %.1e\n",n1,n2,n3,n4);
	printf("%ld %1d\n",n3,n4);
	printf("%ld %ld %ld %ld\n",n1,n2,n3,n4);


	return 0;
}
