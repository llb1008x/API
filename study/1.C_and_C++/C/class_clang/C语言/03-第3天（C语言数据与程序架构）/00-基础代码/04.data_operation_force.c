#include <stdio.h>
int main()
{
	float x=0;
	int i = 0;
	x=3.6f;
	i = x;			//xΪʵ��,iΪ���ͣ�ֱ�Ӹ�ֵ���о���
	i = (int)x;		//ʹ��ǿ������ת��
	printf("x=%f,i=%d\n",x,i);
	return 0;
}
 