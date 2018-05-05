

int strcmp(char *s1, char *s2)
{
	while(*s1 || *s2)
	{
		if((*s1 - *s2)>0)
			return 1;
		else if((*s1-*s2)<0)
		  return -1;
		else
		{
			s1++;
			s2++;
		}
	}
	return 0;
}