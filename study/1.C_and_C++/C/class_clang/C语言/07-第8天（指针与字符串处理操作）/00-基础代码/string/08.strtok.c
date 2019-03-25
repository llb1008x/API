#include <stdio.h>
#include <string.h>
int main()
{
	char str1[]="adc*fvcv*!ebcy!hghbdfg*casdert";
	char *str2="*!";
	char *result[10];
	int num=0,i;
	result[num]=strtok(str1,str2);
	while(result[num]!=NULL)
	{
		num++;
		result[num]=strtok(NULL,str2);
	}
	for(i=0;i<num;i++)
		printf("result[%d]=%s\n",i,result[i]);
	return 0;
}
