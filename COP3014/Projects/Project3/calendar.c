#include <stdio.h>

int main(void) {
	unsigned int start = 7, length = 0, day = 0, col = 0;
	
	do {
		printf("Start day of the week: ");
		scanf("%u", &start);
	} while (start > 6);
	
	do {
		printf("Length of the month: ");
		scanf("%u", &length);
	} while (length < 28 || length > 31);
	length += 1;

	printf("Sun Mon Tue Wed Thu Fri Sat\n");
	printf("--- --- --- --- --- --- ---\n");
	
	while (day < length) {
		for (col = 0; col < 7; ++col) {
			if (day == 0 && col == start) {
				++day;
			}
			if (day < length && day != 0) {
				printf("%-4d", day);
				++day;
			} else {
				printf("%4s", "");
			}
		}
		printf("\n");
	}
	
	printf("--- --- --- --- --- --- ---\n");
	
	return 0;
}
