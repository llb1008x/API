/************************************************************************
 *字符串匹配，通过一定的算法在一串字符之后找到目标字符串
 * *********************************************************************/

#include <stdio.h>
#include <string.h>

int main()
{
	char str1[]="SBBCABCDABABCDABCDABDE";
	char str2[]="AB";
	
	printf("str1->%s\n",str1);
	printf("str2->%s\n",str2);
	printf("sizeof(str1)-->%d\n",sizeof(str1));
	printf("sizeof(str2)-->%d\n",sizeof(str2));
    
    int i,j;
    for(i=0;i<sizeof(str1)-sizeof(str2);i++){
    	
    	printf("i--->%d\n",i);
    	for(j=0;j<sizeof(str2);j++){
    	
    		 if(j == (sizeof(str2)-1)){
    			printf("compatiable for str1 with str2\n");
    			printf("i--->%d\n",i);
    			break;
    		}	
    	
    		printf("j--->%d\n",j);
    		if(str1[j+i]!=str2[j])
    		
    				break;
    		
    	}
    
    }   

    return 0;
}
