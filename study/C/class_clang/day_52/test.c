/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月21日 星期一 20时44分33秒
	
题目：
1.练习结构体数组排序　　 
	从键盘输入5个学生的信息（姓名、学号、成绩）,
	存入一个结构体数组中，计算平均分，并按成绩
	高低排序并输出.	
	
	
 ************************************************************************/

#include <stdio.h>

#define MAX_SIZE	2

typedef struct student{
	
	char name[20];
	int id;
	int score;
	
}STU;

int *sort(int *a,int len)
{
	int i,j;
	int tmp;
	
	for(i=0;i<MAX_SIZE;i++){
	
		for(j=i+1;j<MAX_SIZE;j++){
		
			if(a[i]>a[j]){
			
				tmp=a[i];
				a[i]=a[j];
				a[j]=tmp;
			}
		}
	}
	
	
	return a;
}


int main()
{
/*	
	STU stu[3];
	int i,sum;
	double average;
	
	for(i=0;i<MAX_SIZE;i++){
	
		scanf("%s %d %d",stu[i].name,&stu[i].id,&stu[i].score);
		sum += stu[i].score;
	}
	

	for(i=0;i<MAX_SIZE;i++){
	
		printf("%s,%d,%d\n",stu[i].name,stu[i].id,stu[i].score);
	}
	
	
	average= 1.0*sum/MAX_SIZE;
	printf("%lf,%d\n",average,sum);
*/
	int a[]={1,2,3,4,5,6,7,8,9,3};
	int len=sizeof(a)/sizeof(int);
	int i;
	printf("len->%d\n",len);
	
	int *b=sort(a,len);
	
	for(i=0;i<len;i++){
	
		printf("a[%d]->%d\n",i,b[i]);
	}
	
	
	
	return 0;
}
