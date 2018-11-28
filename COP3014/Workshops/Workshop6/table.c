#include <stdio.h>

int main(void) {
	int tableSize = 0, i = 0, x = 0;
	
	
	printf("Enter your table size: ");
	scanf("%d", &tableSize);
	
	int table[tableSize][tableSize];
	
	while (i < tableSize) {
		x = 0;
		while (x < tableSize) {
			table[i][x] = (i+1)*(x+1);
			++x;
		}
		++i;
	}
	
	i = 0;
	while (i < tableSize + 1) {
		x = 0;
		while (x < tableSize + 1) {
			if (i == 0) {
				if (x > 0) {
					printf("%-5d", x);
				} else {
					printf("   ");
				}
			} else {
				if (x == 0) {
					printf("%-5d", i);
				} else {
					printf("%-5d", table[i-1][x-1]);
				}
			}
			++x;
		}
		printf("\n");
		++i;
	}
	
	return 0;
}
