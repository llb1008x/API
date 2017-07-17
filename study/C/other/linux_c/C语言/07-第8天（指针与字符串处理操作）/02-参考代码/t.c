#include <stdio.h>
#include <string.h>

int main()
{
	char msg_src[]={"+CMGR:REC UNREAD,+8613466630259,98/10/01,18:22:11+00,ABCdefGHI"};
	char *str[5];
	int i=0,cont=0;
	
	str[i] = strtok(msg_src,",");
	while(str[i]!=NULL){
		i++;
		str[i] = strtok(NULL,",");
	}
	// for(;cont<i;cont++)
		// printf("%s\n",str[cont]);
		printf("%s\n",str[0]);
		printf("%s\n",str[2]);
		printf("%s\n",str[3]);
		printf("%s\n",str[4]);
	printf("&str[5]=%p\n",&str[5]);
	printf("&str[4]=%p\n",&str[4]);
	printf("&str[1]=%p\n",&str[1]);
	printf("&i=%p\n",&i);
	printf("&cont=%p\n",&cont);
	
	return 0;
}
