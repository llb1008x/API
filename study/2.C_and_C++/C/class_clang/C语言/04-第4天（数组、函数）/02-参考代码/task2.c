#include <stdio.h>

double tax_calc(double income)
{
	double tax=0,tax_income;
	if(income<0)
		return -1;
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
	double  income,tax;  /* ����������롢Ӧ˰���á���������˰ */ 

	while(1) 
	{
		printf("�������������:\n");
		scanf("%lf",&income); 
		
		tax=tax_calc(income);
		if(tax<0)
			break;
		printf("�������ɵ�˰Ϊ is:%.2f\n",tax);
		printf("������������� is:%.2f\n",income-tax);
	}
	return 0;
} 
