/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月23日 星期二 21时47分23秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
#include <setjmp.h>

#define TOK_ADD     5

jmp_buf jmpbuffer;
char *tok_ptr;


void do_line(char *);
void cmd_add(void);
int get_token(void);


int main()
{
    char line[MAXLINE];
    int i=0;
    int ret=0;
    
    ret=setjmp(jmpbuffer);
    if(ret!=0){
    
        printf("error");
    }
    
    printf("i->%d,ret->%d\n",i++,ret);
    while(fgets(line,MAXLINE,stdin)!=NULL){
    
        do_line(line);
    }

    exit(0);
}


void do_line(char *ptr)
{
    int cmd;
    
    tok_ptr=ptr;
    while((cmd=get_token())>0){
        
        switch(cmd){
        
            case TOK_ADD:
                cmd_add();
                break;
        }
    }
}

void cmd_add(void)
{

    int token;
    
    token=get_token();
    if(token<0){
        
        longjmp(jmpbuffer,1);
    }
}

int get_token(void)
{
    /*for debug*/
}
