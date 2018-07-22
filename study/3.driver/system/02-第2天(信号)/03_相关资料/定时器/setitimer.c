#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/time.h>

void print_msg(int signum)
{
	if(signum==SIGALRM)
		printf("alarm\n");
}

int main(int argc, char *argv[])
{
	signal(SIGALRM, print_msg);	//�Զ����źŴ�����
	
	struct itimerval tick;
	
	tick.it_value.tv_sec = 3;//���ζ�ʱ����ֵ
	tick.it_value.tv_usec = 0;
	tick.it_interval.tv_sec = 1;//�´ζ�ʱȡֵ
	tick.it_interval.tv_usec = 0;
	
	int ret = setitimer(ITIMER_REAL, &tick, NULL);//ITIMER_REAL��ʾ��ʱ���ˣ�����SIGALRM�ź�
	if(ret != 0)
	{
		perror("set timer error");
		return -1;
	}

	while(1);
	return 0;
}