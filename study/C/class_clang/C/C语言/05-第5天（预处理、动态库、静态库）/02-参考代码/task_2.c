#include<stdio.h>
int main(void)
{
    int x,y,z,i=0;
 
    printf("有以下几种方案:\n");
    for(x=1;x<20;x++){
		for(y=1;y<33;y++){
			for(z=1;z<300;z++)
				if((x+y+z)==100&&(15*x+9*y+z)==300)//if((x+y+z)==100&&(5*x+3*y+z/3)==100) 
					printf("方案%d:公鸡数为 %d,母鸡数为 %d,小鸡数为 %d\n",++i,x,y,z);
			}
    }
	/* 
	int i,j,k,n;
	
	for(i=1;i<20;i++)
		for(j=1;j<(100-5*i)/3;j++)
			{
				k = (100-5*i-3*j)*3;
				if(i+j+k == 100)
					debug("公鸡:%d母鸡:%d小鸡:%d\n",i,j,k);
			
			}
	*/
	return 0;
}