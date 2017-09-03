#include <stdio.h>
struct student
{ 
  int   num;
  char  name[20];
  float score;
}Lucy = {101,"Lucy",78},*p = NULL;

int main( )
{
  p = &Lucy;
  printf("num1=%d, num2=%d\n",Lucy.num,p->num);
  printf("name1=%s,name2=%s\n",Lucy.name,p->name);
  printf("score1=%f,score2=%f\n",Lucy.score,(*p).score);
  return 0;
}
