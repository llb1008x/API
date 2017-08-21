/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月21日 星期一 23时44分47秒
	
题目：1
1.练习结构体数组排序　　 
	从键盘输入5个学生的信息（姓名、学号、成绩）,
	存入一个结构体数组中，计算平均分，并按成绩
	高低排序并输出.	
	
 ************************************************************************/

#include <stdio.h>

#define     MAX_SIZE    2

typedef struct student
{
    char name[20];
    int id;
    int score;

}STU;



int *sort(int *a,int len)
{
    int tmp,i,j;

    for(i=0;i<len;i++){


        for(j=i+1;j<len;j++){


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
    STU stu[MAX_SIZE];
    int i,j,tmp,sum=0;
    double average;
    
    
    printf("请输入数据：\n");
    for(i=0;i<MAX_SIZE;i++){
    
        scanf("%s %d %d",stu[i].name,&stu[i].id,&stu[i].score);
        sum += stu[i].score;
    }
    
    average = 1.0*sum/MAX_SIZE;
    printf("%lf\n",average);
    
    //sort(stu,len);
    for(i=0;i<MAX_SIZE;i++){
    
        for(j=i+1;j<MAX_SIZE;j++){
        
            if(stu[i].score>stu[j].score){
            
                tmp=stu[i].score;
                stu[i].score=stu[j].score;
                stu[j].score=tmp;
            }
        }
    }
    
    for(i=0;i<MAX_SIZE;i++){
    
        printf("stu[%d].score->%d\n",i,stu[i].score);
    }



#if     0
    int a[]={1,34,6,2,6,8,3,54};
    int len=sizeof(a)/sizeof(int );
    int i;

    printf("len->%d\n",len);

    sort(a,len);    
    for(i=0;i<len;i++){
        
        printf("a[%d]->%d\n",i,a[i]);
    }

#endif

    return 0;
}
