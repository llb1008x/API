#include  <stdio.h>
#include  <string.h>
fun(char *w , int n)
{
	char t,*s1,*s2;
	s1=w;
	s2=w+n-1;
	while(s1<s2)
	{
		t=*s1;
		*s1=*s2;
		*s2=t;
		s1++;
		s2--;
	}
}
int main(void)
{
	char *p;
	p = "1234567";
	fun(p,strlen(p));
	puts(p);
	return 0;
}
