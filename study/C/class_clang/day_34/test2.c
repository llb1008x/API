#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char str1[10000]="abba";
	char str2[10000]="abba ababbba abbabbabbabbaacc";
	int len1=0,len2=0;
	int index=0,i=0,cnt=0;

	//gets(str1);
	//gets(str2);
	len1=strlen(str1);
	len2=strlen(str2);

	//printf("len1->%d,len2->%d\n",len1,len2);
	//第一个字符串比第二个短，两个字符串介于0~10000之间
	if( (len1>len2)||(len1<=0)||(len2>10000)){
	
		printf("error!\n");
		return -1;
	}

	//循环遍历字符串二
	for(index=0;index<len2-len1;index++){
	
		for(i=0;i<len1;i++){

			if(str2[index+i]==str1[i]){
				if(i==(len1-1)){
					printf("%d ",index);
					cnt++;
					break;
				}
			}
		
			if(str2[index+i]!=str1[i]){

				break;
			}
		}
	}
	//printf("\n");
	
	if(!cnt){
	
		printf("-1\n");
	}

	return 0;
}
