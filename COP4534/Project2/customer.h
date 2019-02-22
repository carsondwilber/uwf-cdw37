#ifndef CUSTOMER_H_
#define CUSTOMER_H_

struct Customer_st {
	float arrivalTime;
	float serviceStarted;
	float departureTime;
	struct Customer_st * nextCustomer;
};

typedef struct Customer_st Customer_t;

Customer_t * createCustomer(float arrivalTime);

#endif /* CUSTOMER_H_ */
