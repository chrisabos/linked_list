#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

// temp
#include <stdio.h>

#include <stdlib.h>

#define LL_SUCCESS 0
#define LL_ERR_INVALID_ARG -1
#define LL_ERR_NODE_CREATE_FAILED -2
#define LL_ERR_INDEX_OUT_OF_BOUNDS -3

typedef struct linked_list linked_list;
typedef struct ll_node ll_node;

struct linked_list
{
	ll_node* head;
	void (*func_free)(void*);
};

linked_list* ll_create(void (*func_free)(void*));
ll_node* _ll_node_create(void* value, ll_node* next);
size_t ll_size(linked_list* llp);
int ll_insert(linked_list* llp, void* value, unsigned int index);
int ll_push(linked_list* llp, void* value);
void* ll_pop(linked_list* llp);
void ll_delete(linked_list* llp);
void ll_print(linked_list* llp);

#endif
