#include <stdio.h>
#include <string.h>

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
	int row;
	if((fp=fopen("test.txt","rb+"))==NULL)
	{
		printf("Cannot open file!");
		return 0;
	}
	printf("name、num、age：\n");
#if 0
	printf("input data\n");
	for(i=0;i<2;i++)
		scanf("%s %d %d",boya[i].name,&boya[i].num,&boya[i].age);
		
	for(i=0;i<2;i++)
		fprintf(fp,"%s %d %d\n",boya[i].name,boya[i].num,boya[i].age);
	//fwrite(boya,sizeof(struct stu),2,fp);
#else	
	//将学生信息写入文件中
	// rewind(fp);	//文件指针经过写操作已经到了最后，需要复位
	//fread(boyb,sizeof(struct stu),2,fp);    //将文件中的数据读入到内存中

	i = 0;
	while(1){
		printf("请输入读取第几行:\n");
		scanf("%d",&row);
		fseek(fp,row*10,SEEK_SET);
		fscanf(fp,"%s %d %d",boya[i].name,&boya[i].num,&boya[i].age);
		printf("%s %d %d\n",boya[i].name,boya[i].num,boya[i].age);
	}
#endif		
	fclose(fp);								
	return 0;
}