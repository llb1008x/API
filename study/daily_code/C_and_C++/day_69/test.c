/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年10月18日 星期三 23时48分56秒
	
	实现简易计算器功能
 ************************************************************************/

#include <stdio.h>
#include <ctype.h>

#define  MAXSIZE    100


double myatof(char *s)
{

    double val,power;
    int i,sign;

    for(i=0;isspace(s[i]);i++)
            ;

    sign=(s[i] == '-')?-1:1;
    if( (s[i]=='+') || (s[i]=='-'))
            i++;

    for(val=0.0;isdigit(s[i]);i++){

        val=10.0*val+(s[i]-'0');
    }

    if(s[i]=='.')
        i++;

    for(power=1.0;isdigit(s[i]);i++){

        val=10.0*val+(s[i]-'0');
        power *= 10.0;
    }
    
    return sign*val/power;
                                                                                                                                                                                                                                                                                                                                                         
}


//将行保存到s中，并返回该行的长度
int mygetline(char *s,int lim)
{
    int i=0,c;
    
    while( (--lim>0) && ((c=getchar()) != EOF ) &&(c!='\n') ){

        s[i++]=c;
    }

    if(c=='\n'){

        s[i++]=c;
    }

    s[i]='\0';

    return i;
}



int main()
{
/*
    char *s="123.456";
    double ret;
    
    ret=myatof(s);
    printf("ret->%lf\n",ret);
*/
    
    double sum,myatof(char []);
    char line[MAXSIZE];
    
    int mygetline(char line[],int max);
    
    sum=0;
    while(mygetline(line,MAXSIZE)>0){
    
        printf("\t%g\n",sum += myatof(line));    
    }
    
    return 0;
    


    return 0;
}

