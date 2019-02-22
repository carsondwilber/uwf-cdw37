#include "customer.h"

#ifndef QUEUE_H_
#define QUEUE_H_

Customer_t * popHead(Customer_t ** head, Customer_t ** tail);

void pushTail(Customer_t ** head, Customer_t ** tail, Customer_t * item);

void pushHead(Customer_t ** head, Customer_t * item);

#endif /* QUEUE_H_ */
