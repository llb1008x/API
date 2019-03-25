/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2019年01月10日 星期四 11时41分51秒
	
	
计算某年某月某日是这年的第几天
 ************************************************************************/


#include <stdio.h>


int main() 
{
    /* 定义需要计算的日期 */
    int year = 2008;
    int month = 8;
    int day = 8;
    int flag=0;
    int i=0;
    
    //判断年份
    if(year%4==0 || (year%100==0 || year%400==0 )){
            printf("闰年\n");
            flag=1;
    }        
    else{ 
            printf("平年\n");        
            flag=0;
    }
    
    //判断月份
    int Month[]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(flag==1)
        Month[1] = 29;
        
    for(i=0;i<month-1;i++)  
            //计算总共多少天          
            day += Month[i];
                     
            
    printf("day=%d,month=%d\n",day,month);            
    
    
    return 0;
}
