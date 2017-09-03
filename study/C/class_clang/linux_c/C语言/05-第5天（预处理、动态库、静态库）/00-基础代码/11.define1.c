#include <stdio.h>
#define PI 3.1415f
int main()
{
	float L,S,R,V;
	printf("Input Radius:");
	scanf("%f",&R);
	L=2.0f*PI*R;
	S=PI*R*R;
	V=4.0f/3*PI*R*R*R;
	printf("L=%.4f,S=%.4f,V=%.4f\n",L,S,V);
	return 0;
}
