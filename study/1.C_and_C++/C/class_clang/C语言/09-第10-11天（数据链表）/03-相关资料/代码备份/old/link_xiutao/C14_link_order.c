#include <stdio.h>
#include <stdlib.h>
void get_file_name(char * dest_file_name,char * src_file_name);

char * read_src_file(unsigned long int *file_length,char *src_file_name);

char * file_text_encrypt(char * src_file_text,unsigned long int length,unsigned int password);

char * file_text_decrypt(char * src_file_text,unsigned long int length,unsigned int password);

void save_file(char* text,unsigned long int length,char * file_name);

void print_help();


int main()
{
	char src_file_name[30];
	char dest_file_name[30];
	char *src_file_text=NULL;
	unsigned long int file_length;
	unsigned int password;
	int key;
	while(1)
	{
		print_help();
		scanf("%d",&key);
		getchar();
		switch(key)
		{
	    	case  1 :
				{
					get_file_name(dest_file_name,src_file_name);// ��ȡԭ�ļ���Ŀ���ļ���
					src_file_text=read_src_file(&file_length,src_file_name);// ��Դ�ļ��ж�ȡ����
					printf("please input your unsigned int passworld:");
					scanf("%d",&password);// �������� �������޷������͵�
					getchar();

					src_file_text=file_text_encrypt(src_file_text,file_length,password);// ���ַ������ܷ��ؼ��ܺ���ַ������׵�ַ
					save_file(src_file_text,file_length,dest_file_name);// ���ַ������浽Ŀ���ļ���
					if(src_file_text!=NULL)
						free(src_file_text);//������Դ
					break;
				}
			case 2:
				{
					get_file_name(dest_file_name,src_file_name);// ��ȡԭ�ļ���Ŀ���ļ���
					src_file_text=read_src_file(&file_length,src_file_name);// ��Դ�ļ��ж�ȡ����
					printf("please input your passworld:");
					
					scanf("%d",&password);// ��ȡ����
					getchar();

					src_file_text=file_text_decrypt(src_file_text,file_length,password);// �����ַ��������ؽ��ܺ���ַ������׵�ַ
					save_file(src_file_text,file_length,dest_file_name);// �����ܺ���ַ������浽Ŀ���ļ���
					if(src_file_text!=NULL)
						free(src_file_text);//������Դ
					break;
				}
			case 3:
				{
					
					return 0;// �˳� ����
					break;
				}
			default :
				{
					printf("������һ����ȷ��ѡ�� \n");
				}
		}
	}
	return 0;
}


/**************************************************************************/
//��������:��ȡ Ŀ���ļ���Դ�ļ�������
//������ src_file_name:Դ�ļ����֡� 
// dest_file_name:Ŀ���ļ�������
/**************************************************************************/
void get_file_name(char * dest_file_name,char * src_file_name)
{
	printf("���������ԭ�ļ�����:");
	gets(src_file_name);
	
	printf("���������Ŀ���ļ�����:");
	gets(dest_file_name);
}



/**************************************************************************/
//��������:�����ļ�����
//������file_length:����ָ�룬�˵�ַ�б����ļ��ֽ����� 
// src_file_name:�ļ����֣��Ӵ��ļ��ж�ȡ���ݡ� 
// ����ֵ:�����ַ������׵�ַ
/**************************************************************************/
char * read_src_file(unsigned long int *file_length,char *src_file_name)
{
	unsigned long int FILE_SIZE;
	char *src_file_text;
	FILE  *fp;

	fp=fopen(src_file_name,"rb");
	if(fp==NULL)
	{
		printf("open %s faild\n",src_file_name);
		return 0;
	}

	fseek(fp,0,SEEK_END);
	FILE_SIZE=ftell(fp);
	*file_length=FILE_SIZE;

	rewind(fp);
	src_file_text=malloc(FILE_SIZE);
	if(fread(src_file_text,FILE_SIZE,1,fp)==0)
	{
		printf("read from src_file faild\n");
		return NULL;
	}
	
	fclose(fp);// �ر��ļ�
		return src_file_text ;
}




/**************************************************************************/
//��������:�����ַ���
//������src_file_text:Ҫ���ܵ��ַ����� length:�ַ����ĳ���  
//   password: ��������
// ����ֵ: ���ܺ���ַ������׵�ַ
/**************************************************************************/
char * file_text_encrypt(char * src_file_text,unsigned long int length,unsigned int password)
{
	char * file_text;
	char  *result_text;
	unsigned int i;

	result_text=file_text=src_file_text;
	
	for(i=0;i<length;i++)
	{
		*file_text++ = (*src_file_text++)+password;
	}
	return result_text;

}


/**************************************************************************/
//��������:�����ַ���
//������src_file_text:Ҫ���ܵ��ַ����� length:�ַ����ĳ���  
//   password: ��������
// ����ֵ: ���ܺ���ַ������׵�ַ
/**************************************************************************/
char * file_text_decrypt(char * src_file_text,unsigned long int length,unsigned int password)
{
	char * file_text;
	char  *result_text;

	unsigned long int i;
	
	result_text=file_text=src_file_text;
	
	
	for(i=0;i<length;i++)
	{
		*file_text++ = (*src_file_text++)-password;
	}
	return result_text;
	
}


/**************************************************************************/
//
//	��������:���ַ������浽Ŀ���ļ���
//������text:Ҫ������ַ����׵�ַ ��   file_name :Ŀ���ļ�������
//    length:�ַ����ĳ���
/**************************************************************************/
void save_file(char* text,unsigned long int length,char * file_name)
{
	FILE  *dest_fp;
	dest_fp=fopen(file_name,"wb+");
	if(dest_fp==NULL)
	{
		printf("open %s faild\n",file_name);
	}

	if(fwrite(text,length,1,dest_fp)<=0)
	{
		printf("write %s error\n",file_name);
	}
	else
	{
		printf("save sucess\n")
	}
	fclose(dest_fp);
}

/**************************************************************************/
//
//	��������:��ӡ������Ϣ
//
//
/**************************************************************************/
void print_help()
{
	printf("********1:�����ļ�***********\n");
	printf("********2:�����ļ�***********\n");
	printf("********3:�˳�����***********\n");
	
}