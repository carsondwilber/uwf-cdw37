#include "customer.h"

#ifndef HEAP_H_
#define HEAP_H_

struct Heap_st {
	Customer_t ** customers;
	int size;
	int maxsize;
};

typedef struct Heap_st Heap_t;

Heap_t * createHeap(int size);

void percolateUp(Heap_t * heap, int index);

void percolateDown(Heap_t * heap, int index);

void add(Heap_t * heap, Customer_t * customer);

Customer_t * popTop(Heap_t * heap);

#endif /* HEAP_H_ */
