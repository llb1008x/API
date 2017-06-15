/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月15日 星期四 08时20分34秒
 ************************************************************************/

#include<stdio.h>
#include "student.h"


void read(FILE *fp,int index)
{
    fseek(fp,index*sizeof(Student),SEEK_SET);
    Student stu;
    if(fread(&stu,sizeof(Student),1,fp)==1){

        printf("第%d个学生：",index+1);
        printf("\t姓名：%s\n",stu.name);
        printf("\t性别：");

        switch(stu.gender){

             case 0:printf("男\n");break;
             case 1:printf("女\n");break;
             case 2:printf("其他\n");break;
         }
        printf("\t年龄：%d\n",stu.age); 
    }

}

int save(Student aStu[],int number)
{
	int ret=-1;
	FILE *fp=fopen("student.data","a+");
    if(fp!=NULL){
        
        fseek(fp,0,SEEK_END);
        long size=ftell(fp);
        int number=size/sizeof(Student);
        int index=0;
        printf("有%d个数量，你要看第几个：",number);
        scanf("%d",&index);
        read(fp,index-1);
        fclose(fp);
    }

    return 0;
}


int main()
{
	int number=0;
	printf("输入学生的数量: ");
	scanf("%d",&number);
	Student aStu[number];
	
    
    return 0;
}
