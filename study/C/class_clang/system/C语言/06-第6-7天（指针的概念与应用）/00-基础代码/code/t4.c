#include <stdio.h>
#include <string.h>
//每次调用strtok第一个参数都不为NULL并切出所有串
int main()
{
	char str1[]="adc*fvcv*!ebcy!hghbdfg*casdert";
	char *str2="*!";
	char *result[10];
	char *src;
	int len = strlen(str1);
	int num=0,i;
	
	result[num]=strtok(str1,str2);
	while(result[num]!=NULL)
	{
		num++;
		src = str1+strlen(result[num])+1;//下一个串的起始地址
		len = len - strlen(result[num])-1;
		if(len == 0)
			break;
		result[num]=strtok(src,str2);
	}
	
	for(i=0;i<num;i++)
		printf("result[%d]=%s\n",i,result[i]);
	for(i=0;i<sizeof(str1);i++){
		if(str1[i] == '\0')
			printf(" ");
		else
			printf("%c",str1[i]);
	}
	printf("\n");
	return 0;
}
