
int strlen(const char * sc)
{
	int num=0;
	while(*sc!='\0')
	{
		sc++;
		num++;
	}
	return num;
}


int strlen(const char * s)
{
	const char *sc;
	for (sc = s; *sc != '\0'; ++sc);	
	return sc - s;
}
