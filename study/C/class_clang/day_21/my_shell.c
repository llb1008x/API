/*************************************************************************
	> File Name: my_shell.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月02日 星期五 11时44分24秒

	my_shell的实现
编写my_shell时根据书本范例前的关键函数及思路方面的说明尽量自己编写,主要实
现方式就是从得到字符串,以空格又分开存入二维数组,然后从"./"  "/bin"  "/usr/bin"
寻找解析出的命令,再调用execvp()函数在创建的子进程上执行命令.

如此，我们就可得到一个结论：当内置命令直接在命令行上执行时，bash就化身为进程在前台
执行后直接返回，所以在这个例子中它就返回到了上层目录；而当内置命令在shell脚本中执行时
，bash fork出的子进程就去执行了内置命令，与bash进程执行无关，所以出现了上图还在本级
目录下的结果。

 ************************************************************************/

#include<stdio.h>
#include<string.h>  
#include<unistd.h>  
#include<stdlib.h>  

void Swap(char **beginn,char **endd)  
{  
    char *tmp=*beginn;  
    *beginn=*endd;  
    *endd=tmp;  
}  

int main()  
{  
    while(1)  
    {  
    printf("[myshell@localhost shell]$");  
    fflush(stdout);  
    char buf[1024];  
    memset(buf,'\0',sizeof(buf));  
    ssize_t _size=read(0,buf,sizeof(buf)-1);//get value from inout  
    buf[_size-1]='\0';  
//  printf("%s\n",buf);  
    char *my_argv[64];  
    int end=strlen(buf)-1;  
    int begin=0;  
    int index=0;  
    while(end>=0)  
    {  
        //end-->begin  
        if(isspace(buf[end-1]))//the head of first request  is empty  
        {  
            my_argv[index++]=&buf[end];  
        }  
        if(isspace(buf[end]))//last is empty  
        {  
            buf[end]='\0';  
  		}  

    }  
    my_argv[index++]=buf;  
    my_argv[index]=NULL;  
    int start=0;  
    end=index-1;  
    //swap my_argv[start] my_argv[end]  
    while(start<end)  
    {  
        Swap(&my_argv[start],&my_argv[end]);  
        ++start;  
        --end;  
    }  
    index=0;  
    for(;my_argv[index]!=NULL;index++)  
    {  
      printf("%s\n",my_argv[index]);  
    }  
    if(strcmp(my_argv[0],"cd")==0)//judge is build-in command?  
    {  
       chdir(my_argv[1]);//menu is change   
       continue;  
    }  
    //is not build-in command,fork process   
    pid_t id=fork();  
    if(id<0)  
    {  
        perror("fork");  
        exit(1);  
    }  
    else if(id==0)  
    {  
        execvp(my_argv[0],my_argv);  
        exit(1);  
    }  
    else   
    {  
        pid_t ret=waitpid(id,NULL,0);  
        if(ret==id)  
        {  
           // printf("wait successful\n");  
        }  
    }  
    }  
    return 0;  
}
