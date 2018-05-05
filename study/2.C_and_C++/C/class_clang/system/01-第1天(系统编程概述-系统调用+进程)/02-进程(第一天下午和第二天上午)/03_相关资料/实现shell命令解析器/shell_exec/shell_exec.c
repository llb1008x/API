
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

void shell(void)
{
	while(1)
	{
		char cmd_buf[200] = "";
		char *argv[20] = {NULL};
		int i = 0;
		
		getcwd(cmd_buf, sizeof(cmd_buf));//获取当前目录保存在字符数组当中。
		printf("[%s@ %s]$", getenv("USER"), 1+strrchr(cmd_buf, '/'));//打印提示符号。
		fgets(cmd_buf, sizeof(cmd_buf), stdin);//获取字符串
		cmd_buf[strlen(cmd_buf)-1] = '\0';   //把最后一位'\n'变成字符串结束标志'\0'
		
		if(cmd_buf[0] == '\0')
			continue;    //如果是输入回车的话，继续while循环
		
		argv[i] = cmd_buf;
		while((argv[i++]=strtok(argv[i]," "))!=NULL);   //字符串按空格切割   
		
		if(fork()==0)  //创建子进程  调用execvp运行外部命令。
		{
			printf("argv[0] = %s\n", argv[0]);
			execvp(argv[0],argv);   
			printf("command no found!\n");
			exit(1);
		}
		wait(NULL);  //父进程等待子进程结束。
	}
}

int main(int argc, char *argv[])
{
	shell();
	return 0;
}


