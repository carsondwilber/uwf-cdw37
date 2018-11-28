#include <stdio.h>

int main(void) {
	unsigned int distance = 0;
	
	printf("Enter your distance: ");
	scanf("%u", &distance);
	
	if (distance <= 100) {
		printf("Cost is $5.00.\n");
	} else if (distance <= 500) {
		printf("Cost is $8.00.\n");
	} else if (distance <= 1000) {
		printf("Cost is $10.00.\n");
	} else {
		printf("Cost is $12.00.\n");
	}
	
	return 0;
}
