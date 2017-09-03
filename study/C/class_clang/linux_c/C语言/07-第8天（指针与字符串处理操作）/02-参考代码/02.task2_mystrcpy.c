#include <stdio.h>
#include <string.h>

char *my_strcpy(char *dest,const char *src)		
{
	char *ret_string = dest; 				  				
	while( *src !='\0' )
		*dest++ = *src++;
	*dest = '\0';
	return ret_string;
}