#include <stdio.h>
#include <stdlib.h>

#include "link.h"

pLINK _add_node(ppLINK phead, pLINK node, pLINK pos)
{
	if (*phead==NULL)				// frist node
	{
		*phead = node;
		node->prev = NULL;
		node->next = NULL;
	}
	else if (pos!=NULL)
	{
		if (pos == *phead)			// insert before head
		{
			node->prev = NULL;
			node->next = *phead;
			(*phead)->prev = node;
			*phead = node;
		}
		else
		{
			(pos->prev)->next = node;
			node->prev = (pos->prev);
			node->next = pos;
			pos->prev = node;
		}
	}
	else if (pos==NULL)
	{
		pLINK pb = *phead;
		while(pb->next!=NULL) 
			pb=pb->next;
		pb->next = node;
		node->prev = pb;
		node->next = NULL;
	}
	return *phead;
}

pLINK _del_node(ppLINK phead, pLINK node)
{
	if (*phead == node)			// is head node
	{
		if (node->next!=NULL)	// and is not last node
		{		
			(node->next)->prev = NULL;
		}
		*phead = node->next;
		free(node);
		return *phead;
	}
	
	if (node->next==NULL)		// is last node
	{
		(node->prev)->next = NULL;
		free(node);
		return *phead;
	}
	
	(node->next)->prev = node->prev;
	(node->prev)->next = node->next;
	
	free(node);
	return *phead;
}


