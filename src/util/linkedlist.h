#pragma once

typedef struct node
{
	void *data;
	struct node *next;
	struct node *prev;
} node;

typedef struct linkedlist
{
	node *head;
	node *tail;
	unsigned count;
	node *index;
}

linkedlist* linkedlist_new(void);
void linkedlist_free(linkedlist*);

void* linkedlist_pop_head(linkedlist*,void);
void* linkedlist_pop_tail(linkedlist*,void);
void* linkedlist_get(linkedlist*,unsigned);
void* linkedlist_get_from_index(linkedlist*,unsigned);

void* linkedlist_iterate(linkedlist*);

void linkedlist_add_head(linkedlist*, void*);
void linkedlist_add_tail(linkedlist*, void*);
