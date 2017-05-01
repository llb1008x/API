/*************************************************************************
	> File Name: pointer.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年04月30日 星期日 16时22分16秒

	指针的使用

in main,pool=10,&pool=		0x7ffdd996731c
in main,bath=5.&bath=		0x7ffdd9967318
in mikado ,pool=10,&pool=	0x7ffdd99672fc
in mikado ,bath=5.&bath=	0x7ffdd99672ec
 ************************************************************************/

#include<stdio.h>

void mikado(int bath)
{
	int pool=10;

	printf("in mikado ,pool=%d,&pool=%p\n",pool,&pool);
	printf("in mikado ,bath=%d.&bath=%p\n",bath,&bath);

}

int main()
{

	int pool=10,bath=5;

	printf("in main,pool=%d,&pool=%p\n",pool,&pool);
	printf("in main,bath=%d.&bath=%p\n",bath,&bath);
 
	mikado(bath);

	return 0;
}

