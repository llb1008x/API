
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

/*******************************************************
*���ܣ�     ��SIGINT�źż��ź�Я����ֵ��ָ���Ľ���
*������		argv[1]�����̺�
			argv[2]�������͵�ֵ��Ĭ��Ϊ100��
*����ֵ��	0
********************************************************/
int main(int argc, char *argv[])
{
	if(argc >= 2)
	{
		pid_t pid,pid_self;
		union sigval tmp;

		pid = atoi(argv[1]);
		if(argc >= 3)
		{
			tmp.sival_int = atoi(argv[2]);
		}
		else
		{
			tmp.sival_int = 100;
		}
		sigqueue(pid, SIGINT, tmp);
		pid_self = getpid();
		printf("pid = %d, pid_self = %d\n", pid, pid_self);
	}
	return 0;
}

