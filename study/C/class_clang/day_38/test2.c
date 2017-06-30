/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月30日 星期五 14时34分56秒
	
	比较两个分数的大小
	
	本题要求编写程序，比较两个分数的大小。


输入格式:

输入在一行中按照“a1/b1 a2/b2”的格式给出两个分数形式的有理数，其中分子和分母全是int类型范围内的正整数。


输出格式：

在一行中按照“a1/b1 关系符 a2/b2”的格式输出两个有理数的关系。其中“>”表示“大于”，“<”表示“小于”，“=”表示“等于”。

注意在关系符前后各有一个空格。


输入样例：

1/2 3/4

输出样例：

1/2 < 3/4

 ************************************************************************/

#include <stdio.h>
#include <string.h>

int main()
{
	char str1[50];
	char str2[50];
	int a1,a2;
	int b1,b2;
	int ret;
	
	scanf("%s",str1);
	scanf("%s",str2);
	
	sscanf(str1,"%d/%d",&a1,&b1);
	sscanf(str2,"%d/%d",&a2,&b2);
	
	ret=a1*b2-a2*b1;
	
	if(ret>0){
	
		printf("%s > %s",str1,str2);
	}else if(ret==0){
	
		printf("%s = %s",str1,str2);
	}else{
	
		printf("%s < %s",str1,str2);
	}
	
	//printf("a1->%d,b1->%d\n",a1,b1);
	//printf("a2->%d,b2->%d\n",a2,b2);
	

	return 0;
}
