#include <stdio.h>
typedef struct stu
{
	int num;
	char name[100];
	char sex;
}STU;
int main(void)
{
	STU boy1,boy2,girl3 = {104,"LiLi",'F'};
	printf("input boy1 num£¬name£¬sex and score\n");
	scanf("%d %s %c",&boy2.num,boy2.name,&boy2.sex);
	boy1 = boy2;
	printf("boy1,Number=%d,Name=%s,Sex=%c\n",boy1.num,boy1.name,boy1.sex);
	printf("girl3,Number=%d,Name=%s,Sex=%c\n",girl3.num,girl3.name,girl3.sex);
	return 0;
}