/*************************************************************************
	> File Name: typewrite_game.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年05月20日 星期六 22时26分48秒
 ************************************************************************/
/************************************************************************
*
*
*
* **********************************************************************/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include <termios.h>
#include <unistd.h>


char mygetch()
{
	struct termios oldt, newt;
	char ch;
	tcgetattr( STDIN_FILENO, &oldt  );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO  );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt  );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt  );

	return ch;

}

		
int main()
{
    srand((unsigned )time(NULL));

    int i=0;
    for(i=0;i<10;i++){

        printf("i->%d,rand()->%d\n",i,rand()%100);
    }

	char ch;
	
	while(1){
			
		ch=mygetch();
		printf("%c",ch);
		fflush(NULL);
	}

    return 0;
}
