#include <termios.h>
#include <stdio.h>
#include <unistd.h>


char mygetch( ) {
    struct termios oldt, newt;
    char ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}


int main()
{
	
	char ch;
	
	ch = mygetch();
	printf("ch=%c\n",ch);
	
	return 0;
}