#include <stdio.h>

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

// int main()
// {
	// int a=10,b=20;
	// int temp;
	
	// printf("a = %d,b = %d\n",a,b);
	// a = a^b;
	// b = a^b;
	// a = a^b;
	// temp  = a;
	// a = b;
	// b = temp;
	// printf("a = %d,b = %d\n",a,b);
	// return 0;
// }


// float x = 3.6f;
// int main()
// {
	
	// int i = 3;
	// int value;
	
	// value = (i++)+(++i);//9,
	// printf("value = %d,i = %d\n",value,i);
	// i = 3;
	// value = (++i)+(++i)+(++i);//16
	// printf("value = %d,i = %d\n",value,i);
	
	// return 0;
// }
