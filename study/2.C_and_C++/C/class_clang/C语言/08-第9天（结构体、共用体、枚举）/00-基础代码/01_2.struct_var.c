#include <stdio.h>
//½á¹¹ÌåÇ¶Ì×
struct date
{
	int year;
	int month;
	int day;	
};

struct stu
{
	int num;
	char name[100];
	char sex;
	float score;
	struct date birthday;
} girl;

int main(void)
{

	printf("Number,name,sex,score:\n");
	
	scanf("%d %s %c %f",&girl.num,girl.name,&girl.sex,&girl.score);	

	printf("girl.year,girl.month,girl.day\n");
	scanf("%d %d %d",&girl.birthday.year,&girl.birthday.month,&girl.birthday.day);


	printf("Number=%d,Name=%s\n",girl.num,girl.name);	
	printf("Sex=%c,Score=%f\n",girl.sex,girl.score);
	printf("girl.birthday.year=%d,girl.birthday.month=%d,girl.birthday.day=%d\n",girl.birthday.year,girl.birthday.month,girl.birthday.day);
	return 0;
}