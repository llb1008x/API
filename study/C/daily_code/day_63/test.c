/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年09月26日 星期二 21时25分50秒
 文件的读写操作
 ************************************************************************/
#include <stdio.h>

#define FILE_NAME   "./work.txt"
#define NUM         2

//定义学生这个数据
//把这个结构体当成一个数据域
struct stu{

    char name[10];
    int num;
    //int age;
}boya[NUM],boyb[NUM];


int main()
{
//1.打开文件
    FILE *fp=NULL;
    int i,len,len1,len2;
    
    fp=fopen(FILE_NAME,"w+");
    if(!fp){
        
        printf("open faild!\n");
        return -1;
    }
    

//2.输入数据
    printf("input data!\n");
    printf("name,num:\n");
    
    for(i=0;i<NUM;i++){
    
        scanf("%s %d",boya[i].name,&(boya[i].num));
    }
    
    //对文件进行读写操作
    len1=fwrite(boya,sizeof(struct stu),NUM,fp);
    len=ftell(fp);
    rewind(fp);
    len2=fread(boyb,sizeof(struct stu),NUM,fp);
    printf("len->%d,len1->%d,len2->%d\n",len,len1,len2);
    
    for(i=0;i<NUM;i++){
    
        printf("%s %d\n",boyb[i].name,boyb[i].num);
    }

 //3.关闭文件指针
    fclose(fp);    
    return 0;
}
