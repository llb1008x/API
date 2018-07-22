#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include<fcntl.h>

int main(void)
{
	char *data;
	char login[10];
	char password[10];
	int i;
	char *lenstr;
	long len;
	char testlen[5]="";
	printf("%s\r\n\r\n","Content-Type:text/html");
	lenstr = getenv("CONTENT_LENGTH");	//post
	//lenstr = getenv("QUERY_STRING");			//get
	sprintf(testlen,"%d",lenstr);
	if(lenstr !=NULL)
	{
		sscanf(lenstr, "%ld", &len);
		printf("len=%d\n",len);
		data = (char *)malloc(len);
		read(0, data, len);		
		printf("data=%s\n",data);
		for(i = 0; i < strlen(data); i++)
			if(data[i] == '&')
				data[i] = ' ';
		if(sscanf(data,"login=%s password=%[0-1]",login, password) != 2)
			printf("<p>Name or PassWD Error!<a href=\"/index.html\">Return/a>");
		else
		{
			if(strcmp(login, "root") == 0 && strcmp(password, "111111") == 0)
				printf("<meta content=\"0; URL=/cgi-bin/pass.cgi?\" http-equiv=\"refresh\">");
			else
				printf("<p>Input Error! ReTry!<a href=\"/index.html\">Return</a>");
		}
		free(data);
	}
	else
	{
		printf("<p>PassWD Error! ReTry<a href=\"/index.html\">Return</a>");
	}
	return 0;
}
																																
