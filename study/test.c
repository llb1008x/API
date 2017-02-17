#include <stdio.h>
#include <string.h>

int main()
{
	char str1[]="SBBCABCDABABCDABCDABDE";
	char str2[]="PABCDABD";
	
	//printf("str1->%s\n",str1);
	//printf("str2->%s\n",str2);
	
	int i=0,j=0;
	
	for(i=0;i<strlen(str1)-strlen(str2);i++){
	
		for(j=0;j<strlen(str2);j++){
		
		
			if( str1[i]!=str2[j])
					break;
			
			if(j==strlen(str2)-1){
				
				printf("i->%d,j->%d\n",i,j);
				return 0;
			}
						
		}
	}

	return 0;
}
