/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月26日 星期日 10时06分59秒
	简易的计算器
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>

//#include "my_string.h"

#define MAXOP   100     //操作数或运算符的最大长度
#define NUMBER  '0'     //标示找到一个数字
#define MAXVAL  100     //栈的最大深度

#define BUFSIZE     100 //输入缓冲的大小

//int number='0';        //标示找到一个数字
int sp=0,bufp=0;     
double val[MAXVAL];     

char buf[BUFSIZE];

//读取一个字符
int getch(void)
{
    //printf("111\n");
    return (bufp>0)?buf[--bufp]:getchar();
}

//把字符压回到输入中
void ungetch(int a)
{
    //printf("222\n");

    if(bufp>=BUFSIZE)
        printf("ungetch :too many characters\n");
    else
        buf[bufp++]=a;
}


/*获取下一个运算符或者操作数*/
int getop(char s[])
{
    //printf("333\n");
    int i,c;
    
    while((s[0]=c=getch()) == ' ' || c == '\t')
    //while( (c=getch() == ' ') || (s[0]=c) || (c == '\t'))  
             ;
    //printf("444\n");               
    s[1]='\0';
    //不是数字
    if(!isdigit(c)&&c!='.'){
    
        return c;
    }
    //printf("555\n");
    i=0;
    //收集整数部分
    if(isdigit(c)){
    
        while(isdigit(s[++i]=c=getch()))
            ;
    }
    //printf("666\n");
    /*收集小数部分*/
    if(c=='.'){
    
        while(isdigit(s[i++]=c=getch()))
            ;
    }
    //printf("777\n");
          
    s[i]='\0';
    if(c!=EOF)
        ungetch(c);
    //printf("888\n");    
              
    return NUMBER;    
}



/*压栈*/
void push(double f)
{
    if(sp<MAXVAL){
        val[sp++]=f;
    }else{
        printf("error:stack full,can't push %g\n",f);
    }
}

/*弹栈*/
double pop(void)
{
    if(sp>0){
        return val[--sp]; 
    }else{
        printf("error:stack empty\n");
        return 0.0;
    }
}


/*task2 : 添加对栈的操作*/

/*打印栈顶的元素*/
void print_stack()
{
    if(sp>0)
        printf("head stack->%lf\n",val[--sp]);
    else
        printf("error:stack empty\n");    
}


/*逆波兰计算器*/
int main()
{
#if 1
    int type;
    double op2,ret;
    char s[MAXOP];
   
    while( (type = getop(s)) != EOF ){
        switch(type){
            case NUMBER:
                push(atof(s));
                break;
            
            case '+':
                push(pop()+pop());
                break;
            
            case '*':
                push(pop()*pop());
                break;
            
            case '-':
                op2=pop();
                push(pop()-op2);
                break;
            
            case '/':
                op2=pop();
                if(op2!=0.0)
                    push(pop()/op2);
                else 
                    printf("error:zero divisor\n");
                break;
            //task1: % 取模运算            
            case '%':
                printf("1111\n");
                op2=pop();
                ret=pop();
                if(op2!=0.0)
                    push( (int)ret%(int)op2 );
                else
                    printf("error:zero divisor\n");    
                
                printf("push->%d,op2->%lf,ret->%lf\n",(int)ret%(int)op2,op2,ret);
                break;

            case '\n':
                printf("%.8g\n",pop());
                break;
                
            default :
                printf("error:unknown command %s\n",s);
                break;
        }
    }
    
#else
    char s[MAXOP];
    int i=0;
    
    while( getop(s) != EOF ){
    
        push(atof(s));
        
        if(i>=5)
            break;
        //print_stack();
        i++;
    }
    print_stack();
    
    printf("******\n");
    for(i=0;val[i]!='\0';i++){
        
        printf("val[%d]->%lf\n",i,val[i]);
    }
    
#endif     
    
    return 0;
}







