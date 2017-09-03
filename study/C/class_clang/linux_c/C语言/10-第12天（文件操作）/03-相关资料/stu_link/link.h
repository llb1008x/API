#ifndef	_LINK_H_
#define _LINK_H_

typedef struct link
{
	struct link *next;
	struct link *prev;
}LINK, *pLINK, **ppLINK;


pLINK _add_node(ppLINK phead, pLINK node, pLINK pos);
pLINK _del_node(ppLINK phead, pLINK node);

#endif

