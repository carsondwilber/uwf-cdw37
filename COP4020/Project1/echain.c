#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "echain.h"

node_t * createNode(char *vName, TYPE type)
{
	// Allocate the new node on the heap
	node_t *newNode = (node_t *)malloc(sizeof(node_t));
	
	// Allocate space for the name
	newNode->name = (char *)malloc(sizeof(char) * (strlen(vName) + 1));
	
	// Copy and properly terminate the name
	strncpy(newNode->name, vName, strlen(vName));
	newNode->name[strlen(vName)] = '\0';
	
	// Set the type as provided
	newNode->type = type;
	
	// Create node as singular; no next node
	newNode->next = NULL;
	
	return newNode;
}

node_t * pushNode(node_t *top, node_t *node)
{
	// Add the node to the front of the list
	node->next = top;

	// Return the node as the front of the list
	// Note: this expects the caller to be assigning
	// 	 the result to the stack, e.x.:
	//  	top = pushNode(top, node)
	return node;
}

node_t * popNode(node_t *list, node_t **dest)
{
	// if list is null, return null
	if (list)
	{
		// Place the popped node in dest
		*dest = list;
		
		// Prepare the new top node for return
		node_t *ret = list->next;
		
		// Remove the popped node from the list
		list->next = NULL;

		// Return the node as the front of the list
		// Note: this expects the caller to be assigning
		// 	 the result to the stack, e.x.:
		//  	stack = popNode(list, dest)
		return ret;
	}
	
	return NULL;
}

void destroyNode(node_t *node)
{
	// if node is null do nothing
	if (node)
	{
		// Remove the node from any list
		node->next = NULL;

		// Free memory containing its name
		free(node->name);

		// Free memory allocated for the node itself
		free(node);
	}
}

void insertNode(node_t *current, node_t *insert)
{
	// Link the inserted node to the tail of the list
	insert->next = current->next;

	// Link the head of the list to the inserted node
	current->next = insert;
}

node_t * traverse(node_t *list)
{
	// if list is null, return null
	if (list)
	{
		// Return the next node in the list
		return list->next;
	}
	
	return NULL;
}
