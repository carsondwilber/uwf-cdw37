#include "customer.h"
#include "queue.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

Heap_t * heap;
Customer_t * qHead;
Customer_t * qTail;
Customer_t * lastServed;

int n;
int M;
float lambda;
float mu;
int serverAvailableCount;

float totalQueueTime = 0.0f;
float totalServiceTime = 0.0f;
float totalEmptyTime = 0.0f;

/**
 * Uses negative exponential curve to generate a new time interval based on
 * the given average-in-interval parameter.
 */
float GenerateNextRandomInterval(float average) {
	unsigned random = rand();
	if (random > 0) { --random; }
	float value = random / ((float) RAND_MAX);
	return -1.0f * (1.0f / average) * log(value);
}

/**
 * Processes the next event in the heap.
 */
void ProcessNextEvent() {
	Customer_t * customer = popTop(heap);

	if (customer) {
		if (customer->departureTime > 0) {
			serverAvailableCount++;

			totalQueueTime += customer->serviceStarted - customer->arrivalTime;
			totalServiceTime += customer->departureTime - customer->serviceStarted;

			pushHead(&lastServed, customer);

			if (qHead) {
				customer = popHead(&qHead, &qTail);

				customer->serviceStarted = lastServed->departureTime;
				customer->departureTime = lastServed->departureTime + GenerateNextRandomInterval(mu);

				add(heap, customer);

				serverAvailableCount--;
			}
		} else {
			if (serverAvailableCount > 0) {
				if (serverAvailableCount == M) {
					totalEmptyTime += customer->arrivalTime;

					if (lastServed) {
						totalEmptyTime -= lastServed->departureTime;
					}
				}

				serverAvailableCount--;

				customer->serviceStarted = customer->arrivalTime;
				customer->departureTime = customer->arrivalTime + GenerateNextRandomInterval(mu);

				add(heap, customer);
			} else {
				pushTail(&qHead, &qTail, customer);
			}
		}
	}
}

/**
 * Creates N new arrival events and adds them to the heap.
 */
void GenerateArrivals(int N) {
	int i = 0;

	float generationTime = 0.0f;

	if (lastServed) {
		generationTime = lastServed->departureTime;
	}

	for (; i < N; ++i) {
		generationTime += GenerateNextRandomInterval(lambda);

		Customer_t * newCustomer = createCustomer(generationTime);

		add(heap, newCustomer);
	}
}

/**
 * Helper function to efficiently calculate the factorial of N.
 */
int factorial(int N)
{
	unsigned int ret = 1;

	while (N > 1) {
		ret *= N--;
	}

	return ret;
}

/**
 * Calculates statistics based on the analytical model and places the results
 * in the given float pointers.
 */
void CalculateStatisticsAnalytical(float * Po, float * L, float * W, float * Lq, float * Wq, float * rho) {
	*Po = (1 / (float) factorial(M)) * pow(lambda / mu, M) * ((M * mu) / ((M * mu) - lambda));

	int i = 0;
	while (i < M) {
		*Po += (1 / (float) factorial(i)) * pow(lambda / mu, i++);
	}

	*Po = 1.0f / *Po;

	*Wq = ((mu * pow(lambda / mu, M)) / (factorial(M - 1) * pow((M * mu) - lambda, 2))) * *Po;

	*W = *Wq + (1.0f / mu);

	*Lq = lambda * *Wq;

	*L = *Lq + (lambda / mu);

	*rho = lambda / (M * mu);
}

/**
 * Calculates statistics based on the simulation and places the results
 * in the given float pointers.
 */
void CalculateStatisticsSimulated(float * Po, float * W, float * Wq, float * rho) {
	*Po = totalEmptyTime / lastServed->departureTime;

	*W = (totalQueueTime + totalServiceTime) / (float) n;

	*Wq = totalQueueTime / (float) n;

	*rho = totalServiceTime / (M * lastServed->departureTime);
}

int main(int argc, char **argv) {
	srand((unsigned) time(0));

	printf("Enter n: ");
	scanf(" %d", &n);

	printf("Enter lambda: ");
	scanf(" %f", &lambda);

	printf("Enter mu: ");
	scanf(" %f", &mu);

	printf("Enter M: ");
	scanf(" %d", &M);

	printf("\nCreating heap...\n");
	heap = createHeap(200);

	printf("Generating first batch of arrivals...\n");
	int remainingArrivals = n;
	GenerateArrivals(5 * M);
	remainingArrivals -= 5 * M;

	serverAvailableCount = M;

	printf("Simulating...\n");

	int loops = 0;

	while (*(heap->customers)) {
		ProcessNextEvent();

		if (remainingArrivals > 0 && heap->size <= M + 1) {
			if (remainingArrivals > M) {
				GenerateArrivals(M);
				remainingArrivals -= M;
			} else {
				GenerateArrivals(remainingArrivals);
				remainingArrivals = 0;
			}
		}
	}

	float Po, L, W, Lq, Wq, rho;

	CalculateStatisticsAnalytical(&Po, &L, &W, &Lq, &Wq, &rho);

	printf("\nAnalytical Model Statistics:\n");
	printf("Po = %f\n", Po);
	printf("L = %f\n", L);
	printf("W = %f\n", W);
	printf("Lq = %f\n", Lq);
	printf("Wq = %f\n", Wq);
	printf("rho = %f\n", rho);

	CalculateStatisticsSimulated(&Po, &W, &Wq, &rho);

	printf("\nSimulated Model Statistics:\n");
	printf("Po = %f\n", Po);
	printf("W = %f\n", W);
	printf("Wq = %f\n", Wq);
	printf("rho = %f\n", rho);

	return 0;
}
