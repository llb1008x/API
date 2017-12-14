/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月14日 星期四 18时54分46秒
 ************************************************************************/

#include <error.h>
#include <limits.h>
#include <sys/param.h>
#include "apue.h"

#ifdef PATH_MAX
static int pathmax=PATH_MAX;
#else 
static int pathmax=0;
#endif 

#define PATH_MAX_GUESS 	1024

static int errno;

static void pr_pathconf(char *mesg,char *path,int name )
{
	long val;
	
	fputs(mesg,stdout);
	errno;
	
	if( (val=pathconf(path,name))<0 ){
		
		if(errno!=0){
		
			err_sys("pathconf error ,path = %s",path);
		}
		
		fputs("(no limit )\n",stdout);
	}else {
	
		printf("%ld\n",val);
	}

}



/*路径的动态分配*/
char *path_alloc(int *size)
{
	char *ptr;
	
	if(pathmax==0){
		
		errno=0;
		if( (pathmax=pathconf("/home/llb/project",_PC_PATH_MAX)) < 0){
		
			if(errno==0){
			
				pathmax=PATH_MAX_GUESS;
			}else{
				
				err_sys("pathconf error for _PC_PATH_MAX");
			}
		}else{
		
			pathmax++;
		}
	}
	
	if( (ptr=malloc(pathmax+1))==NULL){
	
		err_sys("malloc error for pathname ");
	}
	
	if(size != NULL){
		
		*size = pathmax+1;
	}
	
	return (ptr);
}



/*确定文件描述符*/
#ifdef OPEN_MAX 
static int openmax=OPEN_MAX;
#else 
static int openmax=0;
#endif 

#define OPEN_MAX_GUESS	256

int my_open_max(void )
{
	if(openmax==0){
		printf("111\n");
		errno=0;
		if(openmax=sysconf(_SC_OPEN_MAX)<0){
			
			printf("222\n");
			if(errno==0){
				printf("333\n");
				openmax=OPEN_MAX_GUESS;
			}
			else{
				printf("444\n");
				err_sys("sysconf error for _SC_OPEN_MAX");
			}
		}
		printf("555\n");
	}
	
	return (openmax);
}




int main()
{
	int num=2,i;
/*
	char *pl;
	
	pl=path_alloc(&num);	
	
	printf("num->%d\n",num);
	printf("pathmax->%d\n",pathmax);

*/
	my_open_max();
	printf("sysconf(_SC_OPEN_MAX)->%ld,_SC_OPEN_MAX->%d\n",sysconf(_SC_OPEN_MAX),_SC_OPEN_MAX);
	
	for(i=0;i<sysconf(_SC_OPEN_MAX);i++){
	
		close(i);
	}

    exit(0);
}
