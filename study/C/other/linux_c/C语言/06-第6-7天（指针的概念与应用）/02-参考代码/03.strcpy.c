

char *my_strcpy(char *dest,const char *src)		//源指针使用const 修饰
{
	
	char *ret_string = dest; 				  				//注意使用中间指针

	while( *src !='\0' )
	{
		*dest = *src;
		dest++;
		src++;		
	}
	*dest = '\0';											//写法1										
	
	//	while((*dest++=*src++)!='\0');  //写法2
	
	return ret_string;
}
int main(void)
{
	char str1[20] = {" "};
	char *str2 = {"everyone"};

	my_strcpy(str1,str2);

	printf("%s\n",str1);
}