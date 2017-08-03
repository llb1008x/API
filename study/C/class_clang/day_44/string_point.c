#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{

	char str_array[]="i love C_avr!";
	char *str_point="i love C_point!";
	char *str_heap=NULL;
	
	str_heap=(char *)malloc(20);
	strcpy(str_heap,"i love C_heap");
	
	printf("%s,%s,%s\n",str_array,str_point,str_heap);
	printf("%p,%p,%p\n",str_array,str_point,str_heap);
	printf("%ld,%ld,%ld\n",sizeof(str_array),sizeof(str_point),sizeof(str_heap));
	printf("%ld,%ld,%ld\n",strlen(str_array),strlen(str_point),strlen(str_heap));
	
	str_array[0]='y';
	printf("%s\n",str_array);
	
	//*(str_point+0)='y';
	printf("%s\n",str_point);
	
	return 0;
}
