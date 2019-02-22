#include "customer.h"
#include "queue.h"
#include <stdio.h>

/**
 * Removes the front element in the queue of Customers and returns it.
 * Prints a warning and returns a null pointer if the queue was empty.
 */
Customer_t * popHead(Customer_t ** head, Customer_t ** tail) {
	if (*head) {
		Customer_t * tmp = *head;
		*head = (Customer_t *)0;
		return tmp;
	}

	printf("WARNING: No head to pop!");

	return (Customer_t *)0;
}

/**
 * Sets the given tail to the given element. Also sets the given head to the
 * given element if the queue was previously empty.
 */
void pushTail(Customer_t ** head, Customer_t ** tail, Customer_t * item) {
	if (!*tail) {
		*head = item;
	}

	*tail = item;
}

/**
 * Sets the given Customer's #nextCustomer pointer to the head of the given
 * queue (whether or not it is set to NULL) and sets the head to the new
 * Customer.
 */
void pushHead(Customer_t ** head, Customer_t * item) {
	item->nextCustomer = *head;

	*head = item;
}
