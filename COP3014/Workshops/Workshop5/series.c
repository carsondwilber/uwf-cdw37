#include <stdio.h>

int main(void) {
	unsigned int number;
	
	printf("Your number: ");
	scanf("%u", &number);
	
	int i;
	for (i = 1; i <= number; ++i) {
		printf("%d ", i);
	}
	
	printf("\n");
	
	return 0;
}
