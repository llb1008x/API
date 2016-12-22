/*************************************************
 华氏温度转化成摄氏温度
**************************************************/
#include <stdio.h>
#include <time.h>

#define  CELSIUS_TO_FATHR
#define  REVERSE

int main()
{
    double   fathr,celsius;
    double lower,upper,step;

    lower=0.0;    //温度的下限
    upper=300.0;  //温度的上限
    step=20.0;    //步长

    #if defined(CELSIUS_TO_FATHR) 
    
    printf("华氏度转化为摄氏度\n");  
    
    #if defined(REVERSE)
    for(fathr=upper;fathr>lower;fathr-step){
        
        celsius=(5.0/9.0)*(fathr-32.0);
        sleep(1);
        printf("%lf\t%lf\n",fathr,celsius);
    }

    #else

    fathr=lower;
    while(fathr<=upper){

        celsius=(5.0/9.0)*(fathr-32.0);
        printf("%lf\t%lf\n",fathr,celsius);
        fathr += step;

    }
    #endif

    #else

    printf("摄氏度转化为华氏度\n");
    celsius=lower;
    while(celsius<=upper){
    
        fathr=9.0*celsius/5.0+32.0;
        printf("%lf\t%lf\n",celsius,fathr);
        celsius += step;
    }

    #endif

    return 0;
}
