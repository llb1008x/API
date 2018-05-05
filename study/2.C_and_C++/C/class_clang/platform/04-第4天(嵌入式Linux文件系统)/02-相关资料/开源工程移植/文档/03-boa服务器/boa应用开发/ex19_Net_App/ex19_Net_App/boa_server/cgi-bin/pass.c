#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include<fcntl.h>
#include"led.h"

void config(int *led_config,int led_num);

int main()
{
	int led_num=0;
	int led_config[8]={0,0,0,0,0,0,0,0};
	FILE *fp_pass;
	char buf[512]="";
	char *len;
	printf("%s\r\n\r\n","Content-Type:text/html");	
	len = getenv("CONTENT_LENGTH");
	if(len != NULL)	
	{ 
		unsigned int content_len = 0;
		char *data;
		content_len = atoi(len);
		data = malloc(content_len + 1);
		read(0, data, content_len);	
		sscanf(data,"led%d",&led_num);
		free(data);
	}
	else 
	{
		led_num=0;
		//printf("led_num = 0\n");
	}
	config(led_config,led_num);
	
	led_fun(led_config);	
	if( (fp_pass=fopen("pass_tml","r")) == NULL)
	{
		exit(1);
	}
	while(fgets(buf,512,fp_pass))
	{
		if(strncmp(buf,"$IMAGE_AREA1$",12)==0)
		{			
			int i=0;
			for(i=7;i>=4;i--)
			{
				if(led_config[i]==0)
				{
					printf("<img src=\"/led0.html\" width=30 height=30 hspace=45 >");
				}
				else 
				{
					printf("<img src=\"/led1.html\" width=30 height=30 hspace=45 >");
				}
			}
		}
		else if(strncmp(buf,"$IMAGE_AREA2$",12)==0)
		{
			int i=0;
			for(i=3;i>=0;i--)
			{
				if(led_config[i]==0)
				{
					printf("<img src=\"/led0.html\" width=30 height=30 hspace=45 >");
				}
				else 
				{
					printf("<img src=\"/led1.html\" width=30 height=30 hspace=45 >");
				}
			}			
		}
		else 
			printf("%s",buf);	
	}
	return 0;				
}

void config(int *led_config,int led_num)
{
	FILE *fp;
	if((fp=fopen("config.ini","r+"))==NULL)
	{
		printf("<p>fopen Error!<a href=\"/cgi-bin/pass.cgi\">Return/a>");
		exit(1);
	}
	fscanf(fp,"led1=%d,led2=%d,led3=%d,led4=%d,led5=%d,led6=%d,led7=%d,led8=%d",
							led_config,led_config+1,led_config+2,led_config+3,led_config+4,led_config+5,
							led_config+6,led_config+7);
	
	if(led_num > 0)
	{
		if(led_config[led_num-1]==1)
		{
			led_config[led_num-1] = 0;
		}	
		else 
		{
			led_config[led_num-1] = 1;
		}
		fseek(fp,7*(led_num-1)+5,SEEK_SET);
		fprintf(fp,"%d",led_config[led_num-1]);
	}
	fclose(fp);
}
