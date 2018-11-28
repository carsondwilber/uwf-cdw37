#include <stdio.h>

int main(void) {
	int i, x;
	for (i = 1; i < 4; ++i) {
		printf("Row %d: ", i);
		for (x = 1; x < 6; ++x) {
			printf("%d ", x);
		}
		printf("\n");
	}

	return 0;
}
