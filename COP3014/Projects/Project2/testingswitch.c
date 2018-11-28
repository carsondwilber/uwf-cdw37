#include <stdio.h>

int main(void) {
	int num1;
	
	printf("Number: ");
	scanf("%d", &num1);
	
	switch (num1) {
		case 10:
			printf("%d\n", num1*num1);
			break;
		case 9:
			printf("%d\n", num1*num1*num1);
			break;
		case 3:
		case 2:
			printf("%d\n", num1*99);
			break;
		default:
			printf("Input not recognized.\n");
			break;
	}
	
	return 0;
}
