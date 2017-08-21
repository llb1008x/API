/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月19日 星期六 22时57分30秒
	
题目2：
    编写一个函数实现将"12345"变成十进制12345
	    int my_atoi(char *str);	
		
 ************************************************************************/

#include <stdio.h>

int my_atoi(char *str)
{
    int ret,i=0;
    
    while(str[i]!='\0'){
        
        if( (str[i]>='0')&&(str[i]<='9') ){
        
            ret = ret*10+str[i]-'0';
            printf("i->%d,i->%d\n",ret,i);
        }
        
        i++;
    }

    return ret;
}

int main()
{
    char *str="123a56";
    int num=0;
    
    num=my_atoi(str);
    printf("num->%d\n",num);

    return 0;
}
