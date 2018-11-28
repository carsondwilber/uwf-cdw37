#include <stdio.h>

int main(void) {
	
	int number;
	
	printf("Enter your number: ");
	scanf("%d", &number);
	
	switch (number%2) {
		case 0: printf("Your number is even.\n"); break;
		case 1: printf("Your number is odd.\n"); break;
	}
	
	return 0;
}
