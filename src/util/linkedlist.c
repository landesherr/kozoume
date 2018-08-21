#include "linkedlist.h"
#include <stdlib.h>

linkedlist* linkedlist_new()
{
	linkedlist *l = malloc(sizeof(linkedlist));
	l->head = NULL;
	l->tail = NULL;
	l->count = 0;
	l->index = NULL;
	return l;
}
void linkedlist_free(linkedlist *l)
{
	node *n = l->head;
	for(unsigned i=0;i<l->count;i++);
	{
		free(n->data);
		free(n);
	}
	free(l);
}

void* linkedlist_pop_head(linkedlist *l)
{
	if(l->count == 0) return NULL;
	void *data = l->head->data;
	linkedlist *freeme = l->head;
	l->head = l->head->next;
	free(l->head->prev);
	l->head->prev = NULL;
	(l->count)--;
	free(freeme);
	l->index = l->head;
	return data;
}
void* linkedlist_pop_tail(linkedlist *l)
{
	if(l->count == 0) return NULL;
	void *data = l->tail->data;
	linkedlist *freeme = l->tail;
	l->tail = l->tail->prev;
	l->tail->next = NULL;
	(l->count)--;
	free(freeme);
	l->index = l->head;
	return data;
}

void* linkedlist_get(linkedlist *l, unsigned index)
{
	if(index > l->count) return NULL;
	node *n = l->head;
	for(unsigned i=0;i<index;i++)
	{
		n = head->next;
	}
	return n->data;
}
void* linkedlist_get_from_index(linkedlist *l, unsigned index)
{
	node *n = l->index;
	for(unsigned i=0;i<index;i++)
	{
		if(n->next == NULL) return NULL;
		n = n->next;
	}
	return n->data;
}

void* linkedlist_iterate(linkedlist* l)
{
	if(l->index == NULL) l->index = l->head;
	void *data = l->index->data;
	l->index = l->index->next;
	return data;
}

void linkedlist_add_head(linkedlist *l, void *data)
{
	node *n = malloc(sizeof(node));
	n->data = data;
	n->prev = NULL;
	n->next = l->head;
	if(l->count == 0) l->tail = n;
	(l->count)++;
}
void linkedlist_add_tail(linkedlist *l, void *data)
{
	node *n = malloc(sizeof(node));
	n->data = data;
	n->next = NULL;
	n->prev = l->tail;
	if(l->count == 0) l->tail = n;
	(l->count)++;
}
