#include <stdio.h>
//����ָ�뷨��������Ԫ��
int main()
{
	int a[5],i,*pa,*pb;
	pa = a;				   				//�����������׵�ַ
	pb = &a[0];							//����ĵ�һ��Ԫ�صĵ�ַҲ�����׵�ַ
	
	for(i=0;i<5;i++)
	{
		a[i]=i;
		printf("a[%d]=%d\n",i,a[i]);
		printf("a[%d]=%d\n",i,*(a+i));
		printf("a[%d]=%d\n",i,*(pa+i));	
		printf("a[%d]=%d\n",i, *pb++);	
		
	//	printf("a[%d]=%d\n",i, a++);		//aΪ������������ִ��++����
	}
	printf("\n");
}
 