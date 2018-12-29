/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月15日 星期四 08时20分34秒
 ************************************************************************/

#include<stdio.h>
#include "student.h"

void getList(Student aStu[],int number)
{
	char format[STR_LEN];
	int i;
    int t;

	sprintf(format,"%%%ds",STR_LEN-1);
	for(i=0;i<number;i++){
	
		printf("第%d个学生: \n",i);
		printf("\t姓名： ");
		scanf(format,aStu[i].name);
		printf("\t性别： ");
		scanf("%d",&aStu[i].gender);
		printf("\t年龄： ");
        // 因为上一行要求输入的是一个整形，
        // 但是输入的时候包含了回车，所以后面的都不管用
        scanf("%d",&aStu[i].age);
        printf("hello\n");
    }
}

int save(Student aStu[],int number)
{
	int ret=-1;
	FILE *fp=fopen("student.data","a+");
	if(fp==NULL){
		printf("error\n");
	}else{
		ret=fwrite(aStu,sizeof(Student),number,fp);
		fclose(fp);
	}
	return ret==number;
}


int main()
{
	int number=0;
	printf("输入学生的数量: ");
	scanf("%d",&number);
	Student aStu[number];

	getList(aStu,number);
	if(	save(aStu,number) ){
		
		printf("保存成功\n");
	}else {
	
		printf("保存失败\n");
	}

	return 0;
}
