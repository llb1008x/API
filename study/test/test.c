#include <stdio.h>

int main()
{
	printf("hello world\n");
	
	int n=9;
	printf(n>5?"yes\n":"no\n");
	
	
	char s[] = "hello world";  
	char t[] = "aeiou";  
	int i, j, len;  
	for (i = len = 0; s[i]; i++) {  
		for (j = 0; t[j]; j++) {  
		    if (s[i] == t[j])  
		        break;  
		}  
		if (!t[j])  
		    s[len++] = s[i];  
	}  
	s[len] = 0;  
	printf(s); 
	printf("\n"); 

	return 0;
}
