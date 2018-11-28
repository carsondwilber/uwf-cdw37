#include <stdio.h>

int main(void) {
	int i = 5;
	
	printf("While loop: ");
	while (i > 0) {
		printf("%d ", i);
		i--;
	}
	printf("\n");
	
	i = 5;
	
	printf("Do while loop: ");
	do {
		printf("%d ", i);
		i--;
	} while (i > 0);
	printf("\n");
	
	return 0;
}
