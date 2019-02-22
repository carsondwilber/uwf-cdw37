#include "customer.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Helper function to determine what kind of event the given Customer resembles,
 * retrieving the respective priority value.
 */
double getPriority(Customer_t * customer) {
	if (customer->departureTime > 0) {
		return customer->departureTime;
	}

	return customer->arrivalTime;
}

/**
 * Helper function to determine what kind of event the Customer at the given
 * index in the given heap resembles, retrieving the respective priority value.
 */
double getPriorityAtIndex(Heap_t * heap, int index) {
	return getPriority(heap->customers[index]);
}

/**
 * Allocates the memory for a new heap of the given size.
 */
Heap_t * createHeap(int size) {
	Heap_t * heap = (Heap_t *)malloc(sizeof(Heap_t));
	heap->customers = (Customer_t **)malloc(sizeof(Customer_t *) * size);

	int i = 0;
	for (; i < size; ++i) {
		heap->customers[i] = (Customer_t *)0;
	}

	heap->maxsize = size;

	heap->size = 0;

	return heap;
}

/**
 * Swaps two elements in the heap at the given indices.
 */
void swap(Heap_t * heap, int index1, int index2) {
	Customer_t * tmp = heap->customers[index1];
	heap->customers[index1] = heap->customers[index2];
	heap->customers[index2] = tmp;
}

/**
 * Percolates the Customer at the given index in the given heap up to its
 * appropriate position.
 */
void percolateUp(Heap_t * heap, int index) {
	int searchIndex = (index - 1) / 2;
	float percolatingPriorityToCompare;
	float parentPriorityToCompare;

	percolatingPriorityToCompare = getPriorityAtIndex(heap, index);

	while (searchIndex != index) {
		parentPriorityToCompare = getPriorityAtIndex(heap, searchIndex);

		if (parentPriorityToCompare > percolatingPriorityToCompare) {
			swap(heap, searchIndex, index);
			index = searchIndex;
		} else {
			break;
		}

		searchIndex = (index - 1) / 2;
	}
}

/**
 * Percolates the Customer at the given index in the given heap down to its
 * appropriate position.
 */
void percolateDown(Heap_t * heap, int index) {
	int leftChild = (index * 2) + 1;
	int rightChild = leftChild + 1;
	float percolatingPriorityToCompare;
	float leftChildPriorityToCompare;
	float rightChildPriorityToCompare;

	percolatingPriorityToCompare = getPriorityAtIndex(heap, index);

	while (leftChild < heap->size) {
		if (heap->customers[leftChild]) {
			leftChildPriorityToCompare = getPriorityAtIndex(heap, leftChild);

			if (heap->customers[rightChild]) {
				rightChildPriorityToCompare = getPriorityAtIndex(heap, rightChild);

				if (rightChildPriorityToCompare < leftChildPriorityToCompare) {
					if (rightChildPriorityToCompare < percolatingPriorityToCompare) {
						swap(heap, rightChild, index);
						index = rightChild;
						leftChild = (index * 2) + 1;
						rightChild = leftChild + 1;
						continue;
					} else {
						break;
					}
				}
			}

			if (leftChildPriorityToCompare < percolatingPriorityToCompare) {
				swap(heap, leftChild, index);
				index = leftChild;
				leftChild = (index * 2) + 1;
				rightChild = leftChild + 1;
				continue;
			} else {
				break;
			}
		} else {
			break;
		}
	}
}

/**
 * Adds the given Customer pointer to the given Heap at the last index.
 * Prints a warning if the heap is full and the Customer was not added.
 */
void add(Heap_t * heap, Customer_t * customer) {
	int insertionIndex = heap->size;

	if (insertionIndex < heap->maxsize) {
		heap->customers[insertionIndex] = customer;

		heap->size++;

		percolateUp(heap, insertionIndex);

		return;
	}

	printf("WARINING: Heap is full! Cannot add new node.");
}

/**
 * Removes the first element off of the given Heap, percolates elements as
 * needed, and returns a pointer to the removed element.
 */
Customer_t * popTop(Heap_t * heap) {
	if (*heap->customers) {
		int swapIndex = heap->size - 1;

		Customer_t * top = *heap->customers;

		swap(heap, 0, swapIndex);

		heap->customers[swapIndex] = (Customer_t *)0;

		heap->size--;

		if (*heap->customers) {
			percolateDown(heap, 0);
		}

		return top;
	}

	printf("WARINING: Heap is empty! Cannot pop top node.");

	return (Customer_t *)0;
}
