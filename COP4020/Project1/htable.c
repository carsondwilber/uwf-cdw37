#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "echain.h"
#include "htable.h"

node_t **symbolTable = NULL;

int hashString(char *value)
{
	// Get the length of the value
	int l = strlen(value);
	
	// Hash the value if it is not empty
	if (l > 0)
	{
		// Start with the value of the first character
		int hash = (int)value[0];
		
		// For each character in the value...
		int i;
		for (i = 1; i < l; i++)
		{
			// Perform a hash operation based on the character
			hash = ((hash * (int)value[i]) << 3) ^
				((hash + (int)value[i]) >> 2);
		}

		// All hashes must be positive
		if (hash < 0)
		{
			hash = -hash;
		}
		
		// Return the hash
		return hash;
	}
	
	// Return 0 for the empty or null string
	return 0;
}

void createTable()
{
	// Allocate memory for the symbol table on the heap
	symbolTable = (node_t **)malloc(sizeof(node_t *) * BUCKETS);
	
	// For each slot in the table...
	int i;
	for (i = 0; i < BUCKETS; ++i)
	{
		// Properly set the slot to empty
		symbolTable[i] = NULL;
	}
}

void putNode(node_t **table, node_t *node)
{
	// Ignore a null node
	if (node == NULL)
	{
		printf("Warning: attempt to insert NULL node_t into hash table. Ignored.\n");
		return;
	}
	
	// Get the index to place the node at as its hash value,
	// wrapping around the number of slots
	int idx = hashString(node->name) % BUCKETS;
	
	// If there is already a node in the slot...
	if (table[idx])
	{
		// ...push it onto the top of the externally chained list.
		table[idx] = pushNode(table[idx], node);
	}
	else
	{
		// Otherwise, start the external chain for this slot.
		table[idx] = node;
	}
}
