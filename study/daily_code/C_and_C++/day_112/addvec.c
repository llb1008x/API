/*************************************************************************
	> File Name: addvec.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月12日 星期二 09时53分16秒
 ************************************************************************/

int addcnt=0;

void addvec(int *x,int *y,int *z,int n)
{

    int i;

    addcnt++;

    for(i=0;i<n;i++){

        z[i]=x[i]+y[i];
    }
}
