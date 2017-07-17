STU *reverse(STU *head)
 {
     STU *pf,*pb,*r;
     pf=head;
     pb=pf->next;
     
	 while(pb!=NULL)
     {
            r=pb->next;
            pb->next=pf;
            pf=pb;
            pb=r;
      }
     head->next=NULL;
     head=pf;
     return head;
}