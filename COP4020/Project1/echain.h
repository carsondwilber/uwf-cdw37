/**
 * Student Name: Carson Wilber
 * File Name: echain.h
 * Project 1
 *
 * Structures and basic functions for manipulated a linked list.
 */

#include "types.h"

#ifndef ECHAIN_H
#define ECHAIN_H

/**
 * A linked list with a single field, {@code name}.
 */
struct node_st
{
	char *name;		// Name value of the node.
	TYPE type;		// Type of the symbol.
	struct node_st *next;	// Next node in the list.
};

typedef struct node_st node_t;

/**
 * Creates a new node_t with the given {@code vName}.
 *
 * @param vName value to use for new node_t
 * @param type TYPE to use for new node_t
 * @returns new node_t
 */
node_t * createNode(char *vName, TYPE type);

/**
 * Inserts the given node_t {@code node} at the top of the
 * list that starts with the node_t {@code top}.
 *
 * @param top list to push onto
 * @param node node_t to push
 * @returns node (now top of list)
 */
node_t * pushNode(node_t *top, node_t *node);

/**
 * Removes the top node_t from the given node_t list {@code 
 * list} and returns the new top node.
 *
 * @param list node_t list to pop from
 * @param dest pointer to destination to place popped node
 * @returns popped node, or NULL if list is NULL
 */
node_t * popNode(node_t *list, node_t **dest);

/**
 * Frees memory for the given node_t {@code node}, including
 * its name.
 *
 * @param node node_t to free
 */
void destroyNode(node_t *node);

/**
 * Inserts the given node_t {@code insert} inbetween the
 * the node_t {@code current} and its {@code next} node_t.
 *
 * @param current node_t chain to insert into
 * @param insert node_t to insert
 */
void insertNode(node_t *current, node_t *insert);

/**
 * Traverses the given list and returns the next node.
 *
 * @param list node_t list to traverse
 * @returns next node in the list if list is not NULL,
 * 	else returns NULL
 */
node_t * traverse(node_t *list);

#endif // ECHAIN_H
