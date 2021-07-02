#include "linked_list.h"

struct ll_node
{
	void* value;
	ll_node* next;
};

/**
 * \brief This function creates a new linked list.
 *
 * \details This function allocates a new linked list, sets the head to null, and returns a pointer to it.
 *
 * \return A linked list or NULL if error
 */
linked_list* ll_create(void (*func_free)(void*))
{
	linked_list* ll_new = NULL;

	ll_new = calloc(1, sizeof(linked_list));

	if (NULL != ll_new)
	{
		ll_new->head = NULL;
		ll_new->func_free = func_free;
	}

	return ll_new;
}

// allocates a new ll_node and returns it
ll_node* _ll_node_create(void* value, ll_node* next)
{
	if (NULL == value)
	{
		return NULL;
	}

	ll_node* new_node = calloc(1, sizeof(ll_node));

	if (NULL == new_node)
	{
		return NULL;
	}

	new_node->value = value;
	new_node->next = next;

	return new_node;
}

// returns the size of a linked list
size_t ll_size(linked_list* llp)
{
	if (NULL == llp)
	{
		return 0;
	}

	size_t sz = 0;
	ll_node* cur_node = llp->head;

	while(cur_node)
	{
		cur_node = cur_node->next;
		sz++;
	}

	return sz;
}

// allocates a new node, and adds
int ll_insert(linked_list* llp, void* value, unsigned int index)
{
	if (NULL == llp || NULL == value)
	{
		return LL_ERR_INVALID_ARG;
	}

	size_t lls = 0;
	ll_node* new_node = NULL;
	ll_node* cur_node = NULL;
	ll_node* prev_node = NULL;

	// check for index out of bounds
	// TODO: run size calculation in place to increase runtime efficiency
	lls = ll_size(llp);
	if (index > lls)
	{
		return LL_ERR_INDEX_OUT_OF_BOUNDS;
	}

	// insert at 0 is the same as push. fun fact
	if (0 == index)
	{
		return ll_push(llp, value);
	}

	// create new node
	new_node = _ll_node_create(value, NULL);
	if (NULL == new_node)
	{
		return LL_ERR_NODE_CREATE_FAILED;
	}

	// the list is not empty so we have to iterate to the index
	cur_node = llp->head;

	while(index > 1)
	{
		cur_node = cur_node->next;

		if (NULL == cur_node)
		{
			return LL_ERR_INDEX_OUT_OF_BOUNDS;
		}

		index--;
	}

	ll_node* tmp = cur_node->next;
	cur_node->next = new_node;
	new_node->next = tmp;

	return 0;
}

int ll_remove(linked_list* llp, unsigned int index)
{
	if (NULL == llp)
	{
		return LL_ERR_INVALID_ARG;
	}

	if (NULL == llp->head)
	{
		return LL_ERR_INDEX_OUT_OF_BOUNDS;
	}

	int i = 0;
	ll_node* cur_node = NULL;

	cur_node = llp->head;

	for(i = 0; i < index; i++)
	{
		cur_node = cur_node->next;

		if (NULL == cur_node)
		{
			return LL_ERR_INDEX_OUT_OF_BOUNDS;
		}
	}


}

int ll_push(linked_list* llp, void* value)
{
	if (NULL == llp || NULL == value)
	{
		return LL_ERR_INVALID_ARG;
	}

	ll_node* new_node = _ll_node_create(value, llp->head);

	if (NULL == new_node)
	{
		return LL_ERR_NODE_CREATE_FAILED;
	}

	llp->head = new_node;

	return LL_SUCCESS;
}

void* ll_pop(linked_list* llp)
{
	if (NULL == llp || NULL == llp->head)
	{
		return NULL;
	}

	ll_node* rem_node = llp->head;
	void* ret_val = NULL;

	llp->head = rem_node->next;
	ret_val = rem_node->value;

	free(rem_node);

	return ret_val;
}

void ll_delete(linked_list* llp)
{
	if (NULL == llp)
	{
		return;
	}

	while(llp->head)
	{
		void* val = ll_pop(llp);

		if (NULL != val && NULL != llp->func_free)
		{
			llp->func_free(val);
		}
	}

	free(llp);
}

int ll_for_each(linked_list* llp, void (*func)(void*))
{
	if (NULL == llp || NULL == func)
	{
		return LL_ERR_INVALID_ARG;
	}

	ll_node* cur_node = llp->head;

	while(cur_node)
	{
		if(NULL != cur_node->value)
		{
			func(cur_node->value);
		}

		cur_node = cur_node->next;
	}

	return LL_SUCCESS;
}

void test_print(void* value)
{
	printf("%d\n", *((int*)value));
}

int main()
{
	linked_list* llp = NULL;
	int vals[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int insval = 1337;
	int i;

	llp = ll_create(free);

	if (NULL == llp)
	{
		return 0;
	}

	for(i = 0; i < 10; i++)
	{
		int* ip = calloc(1, sizeof(int));

		*ip = i;

		ll_push(llp, ip);
	}

	int* ip = calloc(1, sizeof(int));
	*ip = 1337;
	ll_insert(llp, ip, 10);

	size_t lls = ll_size(llp);

	printf("Linked list size: %d\n", lls);

	ll_for_each(llp, test_print);

	ll_delete(llp);

	return 0;
}
