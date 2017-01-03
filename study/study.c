/**************************************************************
	平常敲的练习代码
***************************************************************/
#include <stdio.h>


//#define FOR_SEVEN       
#define WEEK
//#define STUDENT_SCORE
//#define USE_GETCHAR

int main()
{
    printf("hello world\n");


/*输出100以内能被7整除的数*/    
#if defined(FOR_SEVEN)
    int i=0;

    while(i<100){
        
        //printf("i--->%d\n",i);
        if( (i%7 == 0)&&(i != 0) ){

            printf("%d\n",i);
        }
        i++;
    }


#endif

/*输出一个星期的哪一天*/
#if defined(WEEK)    
    int day;
    
    printf("please input what day is it today?\n");
    do{
	    scanf("%d",&day);
	    
	    switch(day){

		case 1:
		    printf("monday\n");
		    continue;
		case 2:
		    printf("tuesday\n");
		    continue;
		case 3:
		    printf("wednesday\n");
		    continue;
		case 4:
		    printf("thursday\n");
		    continue;
		case 5:
		    printf("friday\n");
		    continue;
		case 6:
		    printf("saturdat\n");
		    continue;
		case 7:
		    printf("sunday\n");
		    continue;
		default:
		    printf("error ,continue\n");
	    }

    }while(day!=0);

#endif

/*根据学生的分数判断等级*/
#if defined(STUDENT_SCORE)
    int score ;

    printf("please input the score of student:\n");
    scanf("%d",&score);

    if( (score<0)||(score>100) ){

        printf("error,please input correct score\n");
    }else if( score<=60 ){

        printf("E\n");
    }else if( score<69 ){

        printf("D\n");
    }else if( score<79 ){

        printf("C\n");
    }else if( score<89 ){

        printf("B\n");
    }else if( score<=100 ){

        printf("A\n");
    }
#endif


#if defined(USE_GETCHAR)    
    //getchar()!=EOF 1?0
    if(getchar()!=EOF){

        printf("1\n");
    }else{

        printf("0\n");
    }
    
    //input and output
    int c;
    while( (c=getchar())!='\n'){

        putchar(c);
    }

#endif    
        
    return 0;
}
