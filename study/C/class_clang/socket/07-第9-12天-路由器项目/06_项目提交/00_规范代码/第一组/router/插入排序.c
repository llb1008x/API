#include <stdio.h>
void print(int a[],int n,int i)
{
	printf("%d\n",i)
	for(int j=0;j<8;j++)
	{
	
		printf("%d",a[j]);
	}
	printf("\n");
}
void insert(int a[],int n)
{
	for(int i=1;i<n;i++)
	{
		if(a[i]<a[i-1])
		{
			int j=i-1;
			int x=a[i];
			a[i]=a[i-1];
			while(x<a[j])
			{
				a[j+1]=a[j];
				j--;
			}
			a[j+1]=x;
		}
		print(a,n,i);
	}
}
int main(int argc,char argv[])
{
	int a[8]={3,1,5,7,2,4,9,6};
	insert(a,8);
	print(a,8,8);
	return 0;
}