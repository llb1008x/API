/*=========================================================================
  �������ƣ�C���Խ׶�
  ����ļ���sub_func.c sub_func.h
  ������������ɶԼ�������������Ŀ���
  ���������
  ά����¼��2010-09-12 v1.0		SunPlusEdu
=========================================================================*/
#include <stdio.h>		//����ϵͳͷ�ļ�
#include <math.h>		
#include "Sub_func.h"	//������д��ͷ�ļ�

int main(void)
{
	float num_test_1 = 2,num_test_2 = 4;

	printf("My SunFunction:\n");
	printf("-------------------------------------------------\n");
	printf("the sum of num_test_1 and num_test_2 is:%f\n",my_sum(num_test_1,num_test_2));	//��a b֮��
	printf("the dec of num_test_1 and num_test_2 is:%f\n",my_dec(num_test_1,num_test_2));	//��a b֮��
	printf("the mux of num_test_1 and num_test_2 is:%f\n",my_mux(num_test_1,num_test_2));   //��a b֮��
	printf("the dvi of num_test_1 and num_test_2 is:%f\n",my_dvi(num_test_1,num_test_2));   //��a b֮��

	printf("\nThe system lib:\n");
	printf("-------------------------------------------------\n");
	printf("the sin of num_test_1 is: %f\n",sin(num_test_1));				//��a������ֵ
	printf("the sin of num_test_1 is: %f\n",atan(num_test_1));			//��a�ķ�����ֵ
	printf("the num_test_1 of num_test_2  pow is: %f\n",pow(num_test_1,num_test_2));		//��a��b����
	printf("the sqrt of num_test_2 is: %f\n",sqrt(num_test_2));			//��a��ƽ����
	
}



