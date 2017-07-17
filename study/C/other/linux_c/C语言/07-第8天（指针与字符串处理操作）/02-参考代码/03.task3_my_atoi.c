
int my_atoi(char * str)
{
	char *temp = str;
	int num=0;
	while (*temp != '\0')
	{
		num = num * 10 + (*temp - '0');
		temp++;
	}
	return num;
}

