#include <stdio.h>

/*
int main()
{
	int arr[10] = {54,2,485,87,4,2,4,5,84,50};
	int i,j;
	
	for(i=0;i<10;i++)
		printf("%d ",arr[i]);
	printf("\n");
	//冒泡法排序：相邻两两比较
	// for(i=0;i<10;i++)
		// for(j=0;j<10-i-1;j++){
			// if(arr[j]>arr[j+1]){
				// arr[j] =arr[j]^arr[j+1];
				// arr[j+1] =arr[j]^arr[j+1];
				// arr[j] =arr[j]^arr[j+1];
			// }	
		// }
	//选择法排序：用假值与其它值分别比较
		for(i=0;i<10;i++)
		for(j=i+1;j<10;j++){
			if(arr[j]<arr[i]){
				arr[j] =arr[j]^arr[i];
				arr[i] =arr[j]^arr[i];
				arr[j] =arr[j]^arr[i];
			}	
		}
	for(i=0;i<10;i++){
		if(i%5 == 0)
			printf("\n");
		printf("%03d ",arr[i]);
	}
	printf("\n");
	return 0;
}



int main()
{
	unsigned int a;
	
	printf("请输入一个整数:\n");
	scanf("%d",&a);
	//1010 0010
	// a = a|0xa2;
	// a = a|((1<<1)|(1<<5)|(1<<7));
	a |= ((1<<1)|(1<<5)|(1<<7));
	printf("置1后的结果：%d\n",a);
	
	printf("请输入一个整数:\n");
	scanf("%d",&a);
	//1011 0011
	//0..0 0100 1100->((1<<2)|(1<<3)|(1<<6))
	//1..1 1011 0011->~((1<<2)|(1<<3)|(1<<6))
	//a&1..1 1011 0011->a&(~((1<<2)|(1<<3)|(1<<6)))
	//a = a&0xb3;//部分适用
	a &= ~((1<<2)|(1<<3)|(1<<6));//12345678->12145618
	printf("清0后的结果：%d\n",a);
	
	return 0;
}

int main()
{	
	double incom,tax_incom,tax;
	
	printf("请输入你的月薪:\n");
	scanf("%lf",&incom);
	tax_incom = incom -3500;
	if(tax_incom <= 0)
		tax = 0;
	else if((tax_incom > 0)&&(tax_incom <= 1500))
		tax = tax_incom*0.03;
	else if((tax_incom > 1500)&&(tax_incom <= 4500))
		tax = tax_incom*0.1-105;
	else if((tax_incom > 4500)&&(tax_incom <= 9000))
		tax = tax_incom*0.2-555;
	else if((tax_incom > 9000)&&(tax_incom <= 35000))
		tax = tax_incom*0.25-1005;
	else if((tax_incom > 35000)&&(tax_incom <= 55000))
		tax = tax_incom*0.3-2755;
	else if((tax_incom > 55000)&&(tax_incom <= 80000))
		tax = tax_incom*0.35-5505;
	else if(tax_incom > 80000)
		tax = tax_incom*0.45-13505;
	printf("你的月薪是%lf,你交的税是%lf，你的实际收入是%lf\n",incom,tax,incom-tax);
	

	return 0;
}
int main()
{
	int year,month,day,total = 0;
	
	printf("请输入年月日:\n");
	scanf("%d%d%d",&year,&month,&day);//2012,5,10   2012 5 10 
	if(month > 2)
		if(((year%4 == 0)&&(year%100 != 0))||(year%400 == 0))
			total++;

	switch(month){
		case 1:
			total += day;
			break;
		case 2:
			total += 31+day;
			break;
		case 3:
			total += 31+28+day;
			break;
		case 4:
			total += 31+28+31+day;
			break;
		case 5:
			total += 31+28+31+30+day;
			break;
		case 6:
			total += 31+28+31+30+31+day;
			break;		
		case 7:
			total += 31+28+31+30+31+30+day;
			break;
		case 8:
			total += 31+28+31+30+31+30+31+day;
			break;		
		case 9:
			total += 31+28+31+30+31+30+31+31+day;
			break;		
		case 10:
			total += 31+28+31+30+31+30+31+31+30+day;
			break;		
		case 11:
			total += 31+28+31+30+31+30+31+31+30+31+day;
			break;		
		case 12:
			total += 31+28+31+30+31+30+31+31+30+31+30+day;
			break;		
	}
	printf("%d %d %d是这一年的第%d天\n",year,month,day,total);
	return 0;
}


int main()
{
	int i=1,j=5,k=3,water_data = 153;

	for(water_data=0;water_data<1000;water_data++){
		i = water_data/100%10;
		j = water_data/10%10;
		k = water_data/1%10;
		if(i*i*i+j*j*j+k*k*k == water_data)
			printf("水仙花数是: %d\n",water_data);
	}
	return 0;
}
int main()
{
	int score;
	int i;

	while(1)
	for(i=0;i<3;i++){
		printf("哥们请输入:\n");
		scanf("%d",&score);
		if((score<0)||(score>100))
			break;
			
		printf("输入合法，进入判断\n");
		if((score<=60)&&(score>=0)){
			printf("结果为E\n");
		}
		else if((score<=69)&&(score>60))
			printf("结果为D\n");
		else if((score<=79)&&(score>=70))
			printf("结果为C\n");
		else if((score<=89)&&(score>=80))
			printf("结果为B\n");
		else if((score<=100)&&(score>=90))
			printf("结果为A\n");
	}
	return 0;
}

int main()
{
	int a=10,b=20;
	int temp;
	
	printf("a = %d,b = %d\n",a,b);
	a = a^b;
	b = a^b;
	a = a^b;
	temp  = a;
	a = b;
	b = temp;
	printf("a = %d,b = %d\n",a,b);
	return 0;
}


float x = 3.6f;
int main()
{
	
	int i = 3;
	int value;
	
	value = (i++)+(++i);//9,
	printf("value = %d,i = %d\n",value,i);
	i = 3;
	value = (++i)+(++i)+(++i);//16
	printf("value = %d,i = %d\n",value,i);
	
	return 0;
}
*/