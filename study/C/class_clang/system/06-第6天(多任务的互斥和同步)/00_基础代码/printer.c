
void printer(char *str)
{
	while(*str!='\0')
	{
		putchar(*str);	
		fflush(stdout);
		str++;
		sleep(1);
	}
	printf("\n"); 
}


