
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

/*******************************************************
*功能：     发SIGINT信号及信号携带的值给指定的进程
*参数：		argv[1]：进程号
			argv[2]：待发送的值（默认为100）
*返回值：	0
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

