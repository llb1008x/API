
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
		
		getcwd(cmd_buf, sizeof(cmd_buf));//��ȡ��ǰĿ¼�������ַ����鵱�С�
		printf("[%s@ %s]$", getenv("USER"), 1+strrchr(cmd_buf, '/'));//��ӡ��ʾ���š�
		fgets(cmd_buf, sizeof(cmd_buf), stdin);//��ȡ�ַ���
		cmd_buf[strlen(cmd_buf)-1] = '\0';   //�����һλ'\n'����ַ���������־'\0'
		
		if(cmd_buf[0] == '\0')
			continue;    //���������س��Ļ�������whileѭ��
		
		argv[i] = cmd_buf;
		while((argv[i++]=strtok(argv[i]," "))!=NULL);   //�ַ������ո��и�   
		
		if(fork()==0)  //�����ӽ���  ����execvp�����ⲿ���
		{
			printf("argv[0] = %s\n", argv[0]);
			execvp(argv[0],argv);   
			printf("command no found!\n");
			exit(1);
		}
		wait(NULL);  //�����̵ȴ��ӽ��̽�����
	}
}

int main(int argc, char *argv[])
{
	shell();
	return 0;
}


