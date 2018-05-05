#include <stdio.h>
#define BIG 1
int main( )
{
	char str[20] = "C Language",C;
	int i = 0;
	while( ( C= str[i++] ) !='\0')
	{ 
		#if  BIG
			if( C>='a' && C<='z')
			C=C-32;
		#else
			if( C>='A'&& C<='Z')
			C=C+32;
		#endif
		printf("%c",C);
	}
	return 0;
}