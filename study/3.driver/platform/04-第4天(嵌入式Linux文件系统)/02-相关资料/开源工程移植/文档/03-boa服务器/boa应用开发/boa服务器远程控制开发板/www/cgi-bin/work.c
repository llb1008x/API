#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *data;
	char login[10];
	char password[10];
	int i;
	char *lenstr; 
	long len;
	printf("%s\r\n\r\n","Content-Type:text/html");
	lenstr = getenv("CONTENT_LENGTH");
	if(lenstr !=NULL)
	{
		sscanf(lenstr, "%ld", &len);
		data = (char *)malloc(len);
		read(0, data, len);

		for(i = 0; i < strlen(data); i++)
			if(data[i] == '&')
				data[i] = ' ';
		if(sscanf(data,"login=%s password=%s",login, password) != 2)
			printf("<p>��������,�뷵����������<a href=\"/work.html\">���ص�¼</a>");
		else
		{
			if(strcmp(login, "root") == 0 && strcmp(password, "111111") == 0)
				printf("<meta content=\"0; URL=/sum.html\" http-equiv=\"refresh\">");
			else
				printf("<p>�������,����������<a href=\"/work.html\">���ص�¼</a>");
		}
		free(data);
	}
	else
	{
		printf("<p>��������,�뷵����������<a href=\"/work.html\">���ص�¼</a>");
	}
	return 0;
}
																																
