#include<stdio.h>
#include<string.h>
#include <stdlib.h>
float add(float a,float b);
float sub(float a,float b);
float mux(float a,float b);
float dive(float a,float b);
float fun(float a,float b,float (*fun)(float,float));
void help();
int exit1(void);