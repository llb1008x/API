/*************************************************************************
	> File Name: test_fifo.c
	> Author: 
	> Mail: 
	> Created Time: 2018年02月14日 星期三 13时45分31秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>


int main()
{
    const char *fifo_name="my_fifo";
    char *file1="data.txt";
    int pipe_fd=-1;
    int data_fd=-1;
    int res=0;
    const int open_mode=O_WRONLY;
    int bytes_sent=0;
    char buffer[PIPE_BUF+1];
    
    if(access(fifo_name,F_OK)==-1){
    
        res=mkfifo(fifo_name,0777);
        if(res!=0){
        
            fprintf(stderr,"could not create fifo %s\n",fifo_name);
            exit(EXIT_FAILURE);
        }
    }
    
    printf("process %d opending fifo O_WRONLY\n",getpid());
    pipe_fd=open(fifo_name,open_mode);
    data_fd=open(file1,O_RDONLY);
    printf("process %d result %d\n",getpid(),pipe_fd);
    
    if(pipe_fd!=-1){
        
        int bytes_read=0;
        bytes_read=read(data_fd,buffer,PIPE_BUF);
        buffer[bytes_read]='\0';
        while(bytes_read>0){
        
            res=write(pipe_fd,buffer,bytes_read);
            if(res==-1){
                fprintf(stderr,"write error on pipe\n");
                exit(EXIT_FAILURE);
            }
            
            bytes_sent += res;
            bytes_read=read(data_fd,buffer,PIPE_BUF);
            buffer[bytes_read]='\0';
        }
        
        
        close(pipe_fd);
        close(data_fd);
    }else 
        exit(EXIT_FAILURE);
        
    printf("process %d finished \n",getpid());    
    
    return 0;
}
