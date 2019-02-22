/**
 * Student Name: Carson Wilber
 * File Name: htable.h
 * Project 1
 *
 * Utilities and fields for managing hash tables.
 */

#include "echain.h"

#ifndef HTABLE_H
#define HTABLE_H

/**
 * Number of buckets in the symbol hash table.
 */
#define BUCKETS 25

/**
 * Global symbol table.
 */
extern node_t **symbolTable;

/**
 * Hashes the given {@code value} with a simple hash function.
 *
 * @param value string to hash
 * @returns int hash value
 */
int hashString(char *value);

/**
 * Creates a new hash table with {@code BUCKETS} nodes.
 */
void createTable();

/**
 * Inserts the given node_t {@code node} into the hash table
 * {@code table}.
 *
 * @param table hash table of node_t to insert into
 * @param node node_t to insert
 */
void putNode(node_t **table, node_t *node);

#endif // HTABLE_H
