#include <stdio.h>
#include <stdlib.h>
int e;
static int f;
int g = 10;
static int h = 10;
int main(int argc, char *argv[])
{
	int a;
	int b = 10;
	static int c;
	static int d = 10;
	char *i = "test";
	char *k = NULL;
	
	printf("&a\t %p\t //�ֲ�δ��ʼ������\n",&a);
	printf("&b\t %p\t //�ֲ���ʼ������\n", &b);
	
	printf("&c\t %p\t //��̬�ֲ�δ��ʼ������\n", &c);	
	printf("&d\t %p\t //��̬�ֲ���ʼ������\n", &d);	
	
	printf("&e\t %p\t //ȫ��δ��ʼ������\n", &e);	
	printf("&f\t %p\t //ȫ�־�̬δ��ʼ������\n", &f);	

	printf("&g\t %p\t //ȫ�ֳ�ʼ������\n", &g);	
	printf("&h\t %p\t //ȫ�־�̬��ʼ������\n", &h);	
	
	printf("i\t %p\t //ֻ������(���ֳ�����)\n", i);	

	k = (char *)malloc(10);
	printf("k\t %p\t //��̬������ڴ�\n", k);
	return 0;
}
