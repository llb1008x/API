/*=========================================================================
  工程名称：C语言阶段
  组成文件：sub_func.c sub_func.h
  功能描述：完成对计算机程序的整体的控制
  程序分析：
  维护记录：2010-09-12 v1.0		SunPlusEdu
=========================================================================*/
#include <stdio.h>		//包含系统头文件
#include <math.h>		
#include "Sub_func.h"	//包含自写的头文件

int main(void)
{
	float num_test_1 = 2,num_test_2 = 4;

	printf("My SunFunction:\n");
	printf("-------------------------------------------------\n");
	printf("the sum of num_test_1 and num_test_2 is:%f\n",my_sum(num_test_1,num_test_2));	//求a b之和
	printf("the dec of num_test_1 and num_test_2 is:%f\n",my_dec(num_test_1,num_test_2));	//求a b之差
	printf("the mux of num_test_1 and num_test_2 is:%f\n",my_mux(num_test_1,num_test_2));   //求a b之积
	printf("the dvi of num_test_1 and num_test_2 is:%f\n",my_dvi(num_test_1,num_test_2));   //求a b之商

	printf("\nThe system lib:\n");
	printf("-------------------------------------------------\n");
	printf("the sin of num_test_1 is: %f\n",sin(num_test_1));				//求a的正弦值
	printf("the sin of num_test_1 is: %f\n",atan(num_test_1));			//求a的反余弦值
	printf("the num_test_1 of num_test_2  pow is: %f\n",pow(num_test_1,num_test_2));		//求a的b次幂
	printf("the sqrt of num_test_2 is: %f\n",sqrt(num_test_2));			//求a的平方根
	
}



