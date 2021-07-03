/// @file linked_list.c

#include <stdlib.h>
#include "linked_list.h"

struct ll_node
{
	void* value;
	ll_node* next;
};

/**
 * @brief This function creates a new linked list.
 *
 * @details This function allocates a new linked list, sets the head to null, and returns a pointer to it.
 *
 * @param[in] func_free a function pointer called to deallocate the values in this list
 *
 * @return A linked list or NULL if error
 * @retval linked_list* allocated linked list pointer
 * @retval NULL if error
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

/**
 * @private
 * @brief This function is used to create nodes on the linked list
 *
 * @details This function allocates a new ll_node and sets the node's variables based on the parameters
 *
 * @param[in] value a pointer to the value that will be assigned to this node
 * @param[in] a pointer to the next node in the list. Can be NULL
 *
 * @return A pointer to an allocated ll_node or NULL if error
 * @retval ll_node* allocated ll_node pointer
 * @retval NULL if error
 */
ll_node* _ll_node_create(void* value, ll_node* next)
{
	// validate parameters
	if (NULL == value)
	{
		return NULL;
	}

	// allocate memory for the new node
	ll_node* new_node = calloc(1, sizeof(ll_node));
	if (NULL == new_node)
	{
		return NULL;
	}

	// set new node's variables
	new_node->value = value;
	new_node->next = next;

	// return it
	return new_node;
}


/**
 * @private
 * @brief This functions gets a ll_node at an index
 *
 * @details This function iterates over the list, and gets the node corresponding to the given index
 *
 * @param[in] llp a pointer to a linked list
 * @param[in] index the index of the node requested
 *
 * @return A pointer to the node corresponding to the index
 * @retval ll_node* the node at the index
 * @retval NULL if invalid linked list pointer or index out of bounds
 */
ll_node* _ll_node_get(linked_list* llp, unsigned int index)
{
	if (NULL == llp)
	{
		return NULL;
	}

	int i;
	ll_node* cur_node = llp->head;

	for(i = 0; i < index; i++)
	{
		if (NULL == cur_node)
		{
			return NULL;
		}

		cur_node = cur_node->next;
	}

	return cur_node;
}

/**
 * @brief Get a value from the list
 *
 * @details This function gets a value from a list
 *
 * @param[in] llp a pointer to a linked list
 * @param[in] index the index of the value to get
 * @param[out] ret a pointer to where the value will be returned to
 *
 * @return This function returns a linked list status code
 * @retval LL_SUCCESS This function performed the operation successfully
 * @retval LL_ERR_INDEX_OUT_OF_BOUNDS The index is out of the bounds of the list
 * @retval LL_ERR_INVALID_ARG The provided parameter(s) are invalid
 */
int ll_get(linked_list* llp, unsigned int index, void** ret)
{
	if (NULL == llp || NULL == ret || NULL != *ret)
	{
		return LL_ERR_INVALID_ARG;
	}

	ll_node* node_at_index = _ll_node_get(llp, index);

	if (NULL == node_at_index)
	{
		return LL_ERR_INDEX_OUT_OF_BOUNDS;
	}

	*ret = node_at_index->value;

	return LL_SUCCESS;
}

/**
 * @brief This function calculates the size of a linked list
 *
 * @details This function iterates over all of the elements in a linked list, in order to calculate the size of the list
 *
 * @param[in] llp a pointer to a linked list
 *
 * @return The size of the given list
 * @retval size_t the size of the list
 * @retval 0 if a NULL list is provided
 */
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

/**
 * @brief Insert a value into a linked list
 *
 * @details This function inserts a value into a linked list at the given index
 *
 * @param[in] llp a pointer to a linked list
 * @param[in] value a pointer to the value to be put onto the list
 * @param[in] index the index where the value should be inserted
 *
 * @return This function returns a linked list status code
 * @retval LL_SUCCESS This function performed the operation successfully
 * @retval LL_ERR_INDEX_OUT_OF_BOUNDS The index is out of the bounds of the list
 * @retval LL_ERR_INVALID_ARG The provided parameter(s) are invalid
 */
int ll_insert(linked_list* llp, void* value, unsigned int index)
{
	// validate parameters
	if (NULL == llp || NULL == value)
	{
		return LL_ERR_INVALID_ARG;
	}

	// variables
	ll_node* prev_node = NULL;
	ll_node* new_node = NULL;

	if (0 != index)
	{
		// get the node before the index we want to insert at
		prev_node = _ll_node_get(llp, index - 1);
		if (NULL == prev_node)
		{
			// because we have already validated that llp is not NULL, the only way
			// _ll_node_get reutrns NULL is if the index is out of bounds
			return LL_ERR_INDEX_OUT_OF_BOUNDS;
		}
	}

	// create the new node
	new_node = _ll_node_create(value, NULL);
	if (NULL == new_node)
	{
		// because we have already validated that value is not NULL, the only
		// way _ll_node_create returns NULL is if the memory allocation failed
		return LL_ERR_NODE_CREATE_FAILED;
	}

	// link the new node into the list
	if (0 == index)
	{
		if (NULL != llp->head)
		{
			new_node->next = llp->head;
		}
		llp->head = new_node;
	}
	else
	{
		new_node->next = prev_node->next;
		prev_node->next = new_node;
	}
	return LL_SUCCESS;
}

/**
 * @brief Remove a value into a linked list
 *
 * @details This function removes a value from a linked list at the given index
 *
 * @param[in] llp a pointer to a linked list
 * @param[in] index the index where a value will be removed from
 *
 * @return This function returns a linked list status code
 * @retval LL_SUCCESS This function performed the operation successfully
 * @retval LL_ERR_INDEX_OUT_OF_BOUNDS The index is out of the bounds of the list
 * @retval LL_ERR_INVALID_ARG The provided parameter(s) are invalid
 */
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

	ll_node* rem_node = NULL;

	if (0 == index)
	{
		rem_node = llp->head;
		llp->head = rem_node->next;

		// free the node and the value
		goto ll_remove_free_rem_node;
	}

	int i = 0;
	ll_node* cur_node = NULL;

	cur_node = llp->head;

	for(i = 0; i < index - 1; i++)
	{
		cur_node = cur_node->next;

		if (NULL == cur_node)
		{
			return LL_ERR_INDEX_OUT_OF_BOUNDS;
		}
	}

	rem_node = cur_node->next;

	if (NULL == rem_node)
	{
		return LL_ERR_INDEX_OUT_OF_BOUNDS;
	}

	// remove the rem node from the link
	cur_node->next = rem_node->next;

ll_remove_free_rem_node:
	if (NULL != llp->func_free)
	{
		llp->func_free(rem_node->value);
		rem_node->value = NULL;
		rem_node->next = NULL;
	}
	free(rem_node);
	rem_node = NULL;

	return LL_SUCCESS;
}

/**
 * @brief Delete a linked list
 *
 * @details This function deletes an entire linked list
 *
 * @param[in] llp a pointer to a linked list
 *
 * @return This function returns a linked list status code
 * @retval LL_SUCCESS This function performed the operation successfully
 * @retval LL_ERR_INVALID_ARG The provided parameter(s) are invalid
 */
int ll_delete(linked_list* llp)
{
	if (NULL == llp)
	{
		return LL_ERR_INVALID_ARG;
	}

	while(llp->head)
	{
		ll_node* rem_node = _ll_node_get(llp, 0);

		if (NULL != rem_node->value && NULL != llp->func_free)
		{
			llp->func_free(rem_node->value);
		}

		llp->head = rem_node->next;
		free(rem_node);
	}

	free(llp);
	llp = NULL;

	return LL_SUCCESS;
}

/**
 * @brief Runs a function on every value in a linked list
 *
 * @details This function iterates over every element in the list, and runs a function on each element
 *
 * @param[in] llp a pointer to a linked list
 * @param[in] func a pointer to the function that should be called for each element
 *
 * @return This function returns a linked list status code
 * @retval LL_SUCCESS This function performed the operation successfully
 * @retval LL_ERR_INVALID_ARG The provided parameter(s) are invalid
 */
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
//
// /// @private

//
// /// @private
//
