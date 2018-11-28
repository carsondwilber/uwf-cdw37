#include <stdio.h>

int main (void) {
	int a, b;
	
	printf("Enter a first number: ");
	scanf("%d", &a);
	
	printf("Enter a second number: ");
	scanf("%d", &b);
	
	if (a % b == 0)
		printf("Message 1.\n");
	else if (b % a == 0)
		printf("Message 2.\n");
	else
		printf("Message 3.\n");
	
	return 0;
}
