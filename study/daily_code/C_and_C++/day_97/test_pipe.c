/*************************************************************************
	> File Name: test_pipe.c
	> Author: 
	> Mail: 
	> Created Time: 2018年02月14日 星期三 11时00分47秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void read_data(int *);

void write_data(int *);

int main()
{
    int pipes[2],rc;
    pid_t pid;
    
    rc=pipe(pipes);
    if(rc==-1){
        
        perror("\npipes\n");
        exit(1);
    }
    
    pid=fork();
    switch(pid){
        
        case -1:
            perror("\nfork\n");
            exit(1);
        case 0:
            printf("11111\n");
            read_data(pipes);
        default:
            printf("22222\n");
            write_data(pipes);        
    }

    return 0;
}


void read_data(int pipes[])
{
    int c,rc;
    
    close(pipes[1]);
    
    while( (rc=read(pipes[0],&c,1))>0){
        
        putchar(c);    
    }
    
    exit(0);
}


void write_data(int pipes[])
{

    int c,rc;
    
    close(pipes[0]);
    
    while((c=getchar())>0){
    
        rc=write(pipes[1],&c,1);
        
            if(rc==-1){
                
                perror("parent write");
                close(pipes[1]);
                exit(1);
            }
    }
    
    close(pipes[1]);
    exit(0);
}
