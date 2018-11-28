#include <stdio.h>

int main(void) {
	int num1;
	
	printf("Number: ");
	scanf("%d", &num1);
	
	if (num1 == 10) {
		printf("%d\n", num1*num1);
	} else if (num1 == 9) {
		printf("%d\n", num1*num1*num1);
	} else if (num1 == 2 || num1 == 3) {
		printf("%d\n", num1*99);
	} else {
		printf("Input not recognized.");
	}
	
	return 0;
}
