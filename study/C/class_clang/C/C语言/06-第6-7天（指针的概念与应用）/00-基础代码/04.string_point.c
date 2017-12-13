#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
	char str_array[]= "I love C_avr!";
	char *str_point = "I love C_point!";
	char *str_heap=NULL;
	
	str_heap = (char *)malloc(20);
	strcpy(str_heap,"I love C_heap");
	
	printf("%s,%s,%s\n",str_array,str_point,str_heap);
	printf("%p,%p,%p\n",str_array,str_point,str_heap);	
 	printf("%d,%d,%d\n",sizeof(str_array),sizeof(str_point),sizeof(str_heap));	
 	printf("%d,%d,%d\n",strlen(str_array),strlen(str_point),strlen(str_heap));	
 
	str_array[0] = 'y';				//可自由修改变量内容
	printf("%s\n",str_array);
	
	//*(str_point+0) = 'y'; 		//不可修改变量内容
	printf("%s\n",str_point);
	return 0;
}

