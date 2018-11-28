#include <stdio.h>

float absolutevalue(float n) {
	if (n < 0) {
		return -n;
	} else {
		return n;
	}
}

int main(void) {
	float absolutevalue(float n);
	
	float f;
	float result;
	
	printf("Enter any float below to get its absolute value.\n");
	printf("Enter 0 to exit at any time.\n");
	
	do {
		printf("Input: ");
		scanf("%f", &f);
		if (f != 0) {
			result = absolutevalue(f);
			printf("Result: %.2f\n", result);
		}
	} while (f != 0);
	
	return 0;
}
