//�򵥵���ĸ������Գ�ʽ
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#define AL  51 //���������ĸ����󳤶�
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
void help()
{
	printf("\n*****************************************");
	printf("\n* ����������޷��˳�!                   *");
	printf("\n* �밴������ĸ�û�����!                 *");
	printf("\n* ���������ʼ����,��������ĸʱ��ʼ��ʱ!*");
	printf("\n* ����������� _ ��ʾ                   *");
	printf("\n*****************************************\n\n");
}

main()
{
	char str[AL] = "";

	char ch;
	int i=0;
	int count=0;
	int k;
	time_t t_start,t_end;

	while(1)
	{
		system("clear");//vc++�µ���system();����ʵ������
		srand((unsigned)time(NULL));//�ɼ���ǰʱ����Ϊ�����rand()�Ŀ�ʼ
		
		for(k = 0;k<AL-1;k++)//��ӡ50��Ӣ���ַ����ڲ���
		{
			str[k] = rand() % 26 +'a' ;
		}
		
		str[AL-1] = '\0';//д���ַ����Ͻ�����־λ
		
		help();
	
		mygetch();
		
		printf("\n%s\n",str);
		count =0;
		for(i=0;i<AL-1;i++)
		{
			ch = mygetch();
			if(i==0)
				t_start = time(NULL);//ȡ��ʼʱ��
			if(ch == str[i])
			{
				printf("%c",ch);
				count++;
			}
			else
			{
				printf("_");//��ӡ�����־
			}
		}
		t_end = time(NULL);//ȡ����ʱ��
		printf("\n�������!\n��ʱ %d s\n��ȷ��%lf %c\n����Esc�˳������������!\n\n",(t_end-t_start),count*100.0/50,'%');
		while(1)
		{
			ch=mygetch();
			if(ch == 27) 
			return 0;//��Ϊesc�ٴ��˳������˳�����
			if(ch==' ')
			break;
		}
	}
}