
char score_judge(float score)
{
	char ch;
	if(score<0 || score >100)
		printf("�����������Ϣ�д�\n");
	else if( score<60)
		ch = 'E';
	else if ( score < 70 )
		ch = 'D';
	else if ( score < 80 )
		ch = 'C';
	else if ( score < 90 )
		ch = 'B';
	else
		ch = 'A';
	return ch;
}