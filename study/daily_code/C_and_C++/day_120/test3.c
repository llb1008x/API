#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
int main(int argc, char const *argv[])
{
	srand((unsigned)time(0));	
	int total,max = 0,cur_sum = 0,lock = 0,num ;	
	int temp_l = 0,temp_e = 0;
	total = rand()%60+150;
	
	for(int i = 0;i < total;i++){
		if(i%6 == 0)  printf("\n\n");
		num = rand()%31 - 15;
		cur_sum += num;
		printf("[%4d]%3d|%5d\t\t",i, num,cur_sum);
		
		//核心部分
		if(max < cur_sum ){
			max = cur_sum;
			if(lock){
				temp_l = i;
				temp_e = i;
				lock = 0;     //更新序列起始位置，并将lock取反
			}
			temp_e = i;
		}
		if (cur_sum < 0)
		{
			cur_sum = 0;
			lock = 1;    //lock取反
		}
	
 
	}
 
	printf("\n%d  %d|max = %d\n",temp_l,temp_e,max );
	return 0;

}
