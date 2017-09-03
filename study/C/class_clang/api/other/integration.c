/****************************************************
			c语言实现积分的计算
			
1.想法上大体没什么错误，具体实现还是不够，功底不够			
****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>      //编译要加 -lm

//#define WORK

#ifdef WORK

/*******************************************************************************
			1.这是最基本的矩形法
*******************************************************************************/		
/*
以下为被积函数的定义，即函数举例
*/ 

double f(double x) 
{
	double y; 

	y=sqrt(4-x*x); 

	return (y);
	
}

int main(int argc,char *argv[])
{
	int i,n; 

/*
n为区间等分的个数，应尽可能大
*/ 

	double a,b,h,s; 

/*
a为积分下限，b为积分上限，h为步长
*/ 

	printf("积分下限a:\n"); 

	scanf("%lf",&a); 

	printf("积分上限b:\n"); 

	scanf("%lf",&b); 

	printf("区间等分个数n :\n"); 

	scanf("%d",&n); 

	h=(b-a)/n; 
	
/*
步长的计算
*/ 

	s=0.5*( f(a)+f(a+h) )*h;

	for(i=1;i<n;i++)
	{
		s=s+0.5*( f(a+i*h)+f(a+(i+1)*h))*h; 
		
		printf("s-->%lf\n",s);

	} 

	printf("函数f(x) 的积分值为s=%10.6f\n",s); 

} 


#else

/*************************************************************************
			有没有一种更为通用的方法，更准确
----->高斯积分	
代码优化：
			1.隐函数怎么用上去呢，这种只能用在显函数
			2.函数图像怎么顺带着画出来	
*************************************************************************/			

#define  N  3

float function_name(float x)
{
	float y=0.0;
	
	y=sqrt(3+2*x+x*x+x*x*x);

	return y;
}

float Gass_Integral(float ( *func)(float x),float a,float b,int n)
{
	//高斯点及其求积系数列表
	float x1[1]={0.0};
	float A1[1]={2};
	float x2[2]={-0.5573503,0.5573503};
	float A2[2]={1,1};
	float x3[3]={-0.7745967,0.0,0.7745967};
	float A3[3]={0.555556,0.888889,0.555556};
	float x4[4]={0.3399810,-0.3399810,0.8611363,-0.8611363};
	float A4[4]={0.6521452,0.6521452,0.3478548,0.3478548};
	float x5[5]={0.0,0.5384693,-0.5384693,0.9061799,-0.9061799};
	float A5[5]={0.5688889,0.4786287,0.4786287,0.2369269,0.2369269};
	
	float *p,*t;
	
	switch(n){
	
		case 1:
			p=x1;
			t=A1;
			break;
			
		case 2:
			p=x2;
			t=A2;
			break;
			
		case 3:
			p=x3;
			t=A3;
			break;
			
		case 4:
			p=x4;
			t=A4;
			break;
			
		case 5:
			p=x5;
			t=A5;
			break;
	
		default:
		printf("input wrong\n");
	}
	
	float g=0;
	int i=0;
	
	for(i=0;i<n;i++){
	
		g += (*func)( (b-a)*p[i]/2+(a+b)/2 )*t[i];
		g *= (b-a)/2;
	}
	
	return g;

}

int main(int argc,char *argv[])
{
	float function_name(float );
	float a,b;
	
	printf("请输入积分上限b\n");
	scanf("%f",&b);
	
	printf("请输入积分下限a\n");
	scanf("%f",&a);
	
	float ans;
	ans=Gass_Integral(function_name,a,b,N);
	
	printf("ans=%f\n",ans);

	return 0;
}


#endif	


/*
double func(double x)
{
	double y=0;
	
	y=4-x*x;
	
	return y;
}


int main(int argc,char *argv[])
{
	printf("hello world\n");

	int n=1000;
	
	double a=1,b=2;
	
	double d=0,i=0,sum=0;
	d=(b-a)/n;			//1.这样算明显有问题，取整过之后，0.000000,a,b也应该是浮点型
	
	printf("***************\n");
	
	printf("d=%f\n",d);
	
						//2.应该是循环的终止条件
	for(i=a;i<b;i+d){
	
		sum += func(i)*d;
		//printf("sum=%f\n",sum);
	}
	
	printf("sum=%f\n",sum);

	
	return 0;
}

*/	