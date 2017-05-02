/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年05月02日 星期二 09时00分02秒
	测试malloc函数

	这是malloc 的函数原型void *malloc(size_t size);，返回的是void *
	也就是说向系统malloc的时候是申请多少个字节，他不知道类型，成功之后
	强制转换我认为或者标记为什么int ，long
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *p=NULL;
	int n=0,i=0;
	printf("请输入:");
	printf("origin p->%p\n",p);
	scanf("%d",&n);

	p=(int *)malloc(n*sizeof(int ));
	for(i=0;i<n;i++){
	
		scanf("%d",&p[i]);
	}

	for(i=n-1;i>=0;i--){
	
		printf("p[%d]=%d\n",i,p[i]);
	}
		
	printf("now p->%p\n",p);
	free(p);

	return 0;
}
