#include "customer.h"
#include <stdlib.h>

/**
 * Allocates memory for a new user with the given arrival time.
 */
Customer_t * createCustomer(float arrivalTime) {
	Customer_t * newCustomer = (Customer_t *)malloc(sizeof(Customer_t));

	newCustomer->nextCustomer = (Customer_t *)0;

	newCustomer->arrivalTime = arrivalTime;

	newCustomer->serviceStarted = 0;

	newCustomer->departureTime = 0;

	return newCustomer;
}
