/*************************************************************************
	> File Name: link.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月27日 星期日 21时42分09秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "link.h"

typedef struct _list{

    NODE *head;
}LIST;

void link_add(LIST *list,int number)
{
    NODE *p_new=NULL,*pb=list->head;

    p_new=(NODE *)malloc(sizeof(NODE));
    p_new->value=number;

    if(list->head==NULL){
        printf("11111111111\n");
        list->head=p_new;
    }else{

        printf("2222222222\n");
        while(pb->next!=NULL){

            pb=pb->next;
        }
        printf("3333333333\n");
        pb->next=p_new;
        p_new->next=NULL;
    }

}

void link_print(LIST list)
{
    NODE *p=list.head;
#if  1    
    while(p!=NULL){

        printf("%d\n",p->value);
        p=p->next;
    }
#else
    for(p=list.head;p;p=p->next){
        
        printf("%d\n".p->next);
    }
#endif    
}


NODE *link_search(LIST list,int number)
{
    NODE *p=NULL;

    for(p=list.head;p;p=p->next){

        if(p->value==number){

            printf("找到了\n");
            return p;
        }
    }

    if(p==NULL){

        printf("没找到\n");
    }

}

#if 1
NODE *link_remove(LIST list,int number)
{
    NODE *pa=NULL,*pb=list.head;

    while(pb!=NULL){

        if(pb->value==number){

            if(pa){

                pa->next=pb->next;
            }else {

                list.head=pb->next;
            }

            free(pa);
            break;
        }
    }    
    
    return list.head;
}
#endif 

NODE *link_free(LIST list)
{
    
    NODE *pa,*pb;
    pa=list.head;

    while(pa!=NULL){

        pb=pa;
        pa=pa->next;
        free(pb);
    }

    return NULL;
}

int main()
{
    LIST list;
    list.head=NULL;
    int number,isfound;

    do{

        scanf("%d",&number);
        if(number!=-1){

           link_add(&list,number);
        }

    }while(number!=-1);

    printf("aaaaaa\n");
    NODE *p;
    scanf("%d",&number);
    link_search(list,number);
#if     0    
    for(p=list.head;p;p=p->next){
    

    }
#endif
    link_print(list);

    return 0;
}

