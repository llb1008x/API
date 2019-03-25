/*************************************************************************
	> File Name: multvec.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月12日 星期二 09时55分24秒
 ************************************************************************/

int multcnt=0;

void multvec(int *x,int *y,int *z,int n)
{
    int i;
    
    multcnt++;

    for(i=0;i<n;i++)
        z[i]=x[i]*y[i];

}
