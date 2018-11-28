#include <stdio.h>

void switchValue(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int getMax(int* list, int n) {
	int i, p = 0;
	for (i = 1; i < n; ++i) {
		if (list[i] > list[p]) {
			p = i;
		}
	}
	return p;
}

void printArray(int* list, int n) {
	int i = 0;
	for (i = 0; i < n; ++i) {
		printf("%d ", list[i]);
	}
	printf("\n");
}

int main(void) {
	int n, i, temp;
	
	printf("Enter length of array: ");
	scanf("%d", &n);
	
	int list[n];
	
	for (i = 0; i < n; ++i) {
		printf("Enter value for %d: ", i);
		scanf("%d", &list[i]);
	}
	
	printf("Sorting...\n");
	
	for (temp = n; temp > 0; --temp) {
		switchValue(&list[getMax(list, temp)], &list[temp-1]);
	}
	
	printArray(list, n);
	
	return 0;
}
