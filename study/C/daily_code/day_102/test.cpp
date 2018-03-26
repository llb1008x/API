#include <stdio.h>


struct structA
{
    int iMember;
    char *cMember;
};
 
int main(int argc, char* argv[])
{
    structA instant1,instant2;

    char c = 'a';

    instant1.iMember = 1;

    instant1.cMember = &c;

    instant2 = instant1;

    printf("*(instant1.cMember)->%c\n",*(instant1.cMember));

    *(instant2.cMember) = 'b';
    printf("*(instant1.cMember)->%c\n",*(instant1.cMember));

    return 0;
}
