#include <stdio.h>
struct stu
{
	char name[10];
	int num;
	int age;
}boya[2],boyb;
main()
{
	FILE *fp;
	int i;
	if((fp=fopen("test.txt","wb+"))==NULL)
	{
		printf("Cannot open file!");
		return 0;
	}
	printf("name¡¢num¡¢age\n");
	for(i=0;i<2;i++)
		scanf("%s %d %d",boya[i].name,&boya[i].num,&boya[i].age);
	fwrite(boya,sizeof(struct stu),2,fp);
	fseek(fp,sizeof(struct stu),SEEK_SET);
	fread(&boyb,1,sizeof(struct stu),fp);    
	printf("%s %d %d\n",boyb.name,boyb.num,boyb.age);
	fclose(fp);								
	return 0;
}
