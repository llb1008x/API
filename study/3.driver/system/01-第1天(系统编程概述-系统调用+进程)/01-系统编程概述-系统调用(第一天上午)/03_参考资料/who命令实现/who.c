#include <stdio.h>
#include <unistd.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>

/******************************************
*1：定义一个utmp结构体，保存读到的数据。
*2：只读方式打开 /var/run/utmp 文件。
*3：循环调用(read) 读取数据，直到读完utmp文件。  
*4：每次读完数据后判断ut_type类型
*	是一般进程的话(ut_type == USER_PROCESS)
*	打印登陆用户名，终端名，登陆时间
*5：注意：打印时间的时候用ctime去转换。
*******************************************/

int main(int argc, char *argv[])
{
	int utmp_fd;
	
	utmp_fd = open("/var/run/utmp", O_RDONLY);
	while(1)
	{
		int ret;
		struct utmp result;
	
		ret = read(utmp_fd, &result, sizeof(struct utmp));
		if(ret == sizeof(struct utmp))
		{
			if(result.ut_type == USER_PROCESS)
			{
				printf("%s %s %s\n", result.ut_user, 
					result.ut_line, ctime(&result.ut_tv.tv_sec));
			}
		}
		else
		{
			break;
		}
	}
	close(utmp_fd);
	return 0;
}

