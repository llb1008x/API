/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月28日 星期三 16时25分23秒
	
	中国大学慕课网
	第三周：
	排序（10分）
	题目内容：

	程序读入一个正整数n（0<n<=100000），然后读入n个整数，均为32位下的整数。
	输出对这个整数排序后的结果，每个整数后面有一个空格。
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

void sort(int a[],int len)
{
	int i,j;
	int tmp;

	for(i=0;i<len;i++){
		for(j=i+1;j<len;j++){
		
			if(a[i]>a[j]){
				tmp=a[i];
				a[i]=a[j];
				a[j]=tmp;
			}
		}
	}
}

int main()
{
	
	int num,i;
	//printf("请输入要创建几个数据:");
	scanf("%d",&num);
	if((num<=0)||(num>100000)){
	
		printf("error\n");
		return -1;
	}

	int *Pnum=(int *)malloc(sizeof(int)*num);
	if(Pnum==NULL){
	
		printf("error!\n");
		return -1;
	}

	for(i=0;i<num;i++){
	
		scanf("%d",Pnum+i);
	}

	sort(Pnum,num);
	for(i=0;i<num;i++){
	
		printf("%d ",*(Pnum+i));
	}

	free(Pnum);
	
	return 0;
}
