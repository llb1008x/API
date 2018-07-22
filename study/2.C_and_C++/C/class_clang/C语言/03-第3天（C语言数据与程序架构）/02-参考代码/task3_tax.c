#include <stdio.h>
double tax_calc(double income)
{
	double tax_income,tax;
	tax_income=income-3500; 
	if(tax_income<=0) 
	   tax=0; 
	else if(tax_income<=1500) 
	   tax=tax_income*0.03;
	else if(tax_income>1500 && tax_income<=4500) 
	   tax=tax_income*0.1-105;
	else if(tax_income>4500 && tax_income<=9000) 
	   tax=tax_income*0.20-555;
	else if(tax_income>9000 && tax_income<=35000) 
	   tax=tax_income*0.25-1005;
	else if(tax_income>35000 && tax_income<=55000) 	  
		tax=tax_income*0.30-2755;
	else if(tax_income>55000 && tax_income<=8000) 
	   tax=tax_income*0.35-5505;
	else if(tax_income>80000) 
	   tax=tax_income*0.45-13505;
	  return tax;
}
int main() 
{  
	double  income,tax_income,tax;  /* 定义变量收入、应税所得、个人所得税 */ 

	while(1)
	{
		printf("请输入你的收入:\n");
		scanf("%lf",&income); 
		if(income<0)
			break;
	
		tax=tax_calc(income);
		
		printf("你所缴纳的税为 is:%.2f\n",tax);
		printf("你的最终收入是 is:%.2f\n",income-tax);
	}
	return 0;
} 
