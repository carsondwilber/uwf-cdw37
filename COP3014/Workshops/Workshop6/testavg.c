#include <stdio.h>

int main(void) {
	float points[30];
	int i = 0;
	
	while (i < 30) {
		printf("Enter student's score: ");
		scanf("%f", &points[i]);
		++i;
	}
	
	float total;
	i = 0;
	while (i < 30) {
		total += points[i];
		++i;
	}
	total /= 30.0;
	
	printf("The average score on the test was: %f\n", total);
	
	return 0;
}
