/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月27日 星期六 15时16分24秒
 ************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "apue.h"

int my_system(const char *cmdstring)
{
    pid_t pid;
    int status;

    if(cmdstring==NULL){

        return(1);
    }

    if((pid=fork())<0){

        status=-1;
    }else if(pid==0){

        execl("/bin/sh","sh","-c",cmdstring,NULL);
        _exit(127);
    }else {

        while(waitpid(pid,&status,0)<0){

            if(errno!=EINTR){

                status=-1;
                break;
            }
        }
    }

    return(status);
}


int main(int argc,char **argv)
{
    int status;
    
    if((status=my_system("date"))<0){
        
        err_sys("system error!");
    }
    pr_exit(status);

    if((status=my_system("nosuchcommand"))<0){

        err_sys("syste, error!");
    }
    pr_exit(status);

    if((status=my_system("who;exit 44"))<0)
        err_sys("system error!");
    
    pr_exit(status);

    return 0;
}
