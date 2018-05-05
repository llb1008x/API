/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年05月20日 星期六 16时04分42秒
    二维数组的使用
 ************************************************************************/

#include <stdio.h>


int main()
{
    float a[5][3]={{80,75,60},
                   {59,65,71},
                   {59,67,82},
                   {76,80,69},
                   {85,93,88}
                  };

    int i,j,person_low[3]={0};
    float s=0,lesson_aver[3]={0};

    for(i=0;i<3;i++){

        for(j=0;j<5;j++){

            s=s+a[j][i];
            if(a[j][i]<60){

                person_low[i]++;
                printf("person_low->%d\n",person_low[i]);
            }
        }

     lesson_aver[i]=s/5;
     s=0;
    }

    printf("各科平均成绩:\n");
    for(i=0;i<3;i++){
        
         printf("%.2f\n",lesson_aver[i]);   
    }

    printf("各科不及格人数：\n");
    for(i=0;i<3;i++){

        printf("%d\n",person_low[i]);
    }


    return 0;
}
