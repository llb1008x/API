
void link_free(STU * head)
{
	STU * pb;
	pb=head;
	while(head!=NULL)
	{
		head=head->next;
		free(pb);
		pb=head;
	}
}