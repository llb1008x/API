#include <stdio.h>
struct stu
{
	char name[10];
	int num;
	int age;
}boya[10],boyb[2];
int main()
{
	FILE *fp;
	int i;
	if((fp=fopen("test.txt","wb+"))==NULL)
	{
		printf("Cannot open file!");
		return 0;
	}
	printf("input data\n");
	printf("name��num��age��addr��\n");
	for(i=0;i<2;i++)
		scanf("%s %d %d",boya[i].name,&boya[i].num,&boya[i].age);
		
	fwrite(boya,sizeof(struct stu),2,fp);	  //��ѧ����Ϣд���ļ���
	rewind(fp);	//�ļ�ָ�뾭��д�����Ѿ����������Ҫ��λ
	fread(boyb,sizeof(struct stu),2,fp);    //���ļ��е����ݶ��뵽�ڴ���

	for(i=0;i<2;i++)
		printf("%s %d %d\n",boyb[i].name,boyb[i].num,boyb[i].age);
	fclose(fp);								
	return 0;
}