//	__FILE__			宏所在文件的源文件名 
//	__LINE__			宏所在行的行号
//	__DATE__			代码编译的日期
//	__TIME__			代码编译的时间
#include <stdio.h>
int main(void)
{

	printf("%s\n",__FILE__);
	printf("%d\n",__LINE__);
	printf("%s\n",__DATE__);
	printf("%s\n",__TIME__);
	return 0;
}