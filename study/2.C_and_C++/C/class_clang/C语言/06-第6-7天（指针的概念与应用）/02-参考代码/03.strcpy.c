

char *my_strcpy(char *dest,const char *src)		//Դָ��ʹ��const ����
{
	
	char *ret_string = dest; 				  				//ע��ʹ���м�ָ��

	while( *src !='\0' )
	{
		*dest = *src;
		dest++;
		src++;		
	}
	*dest = '\0';											//д��1										
	
	//	while((*dest++=*src++)!='\0');  //д��2
	
	return ret_string;
}
int main(void)
{
	char str1[20] = {" "};
	char *str2 = {"everyone"};

	my_strcpy(str1,str2);

	printf("%s\n",str1);
}