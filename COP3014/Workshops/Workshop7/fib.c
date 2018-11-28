#include <stdio.h>
#define N 1024

int fib[N];

void initArray(int n) {
	int i;
	for (i = 0; i < n; ++i) {
		fib[i] = 0;
	}
}

int computeFib(int n) {
	initArray(n);
	int m = 1, i = 0;
	for (i = 1; m <= n; ++i) {
		fib[i] = m;
		m += fib[i-1];
	}
	return i;
}

void printArray(int n) {
	int i;
	for (i = 0; i < n; ++i) {
		printf("%d ", fib[i]);
	}
	printf("\n");
}

int main(void) {
	int n;
	
	printf("Fibonacci up to: ");
	scanf("%d", &n);
	
	printArray(computeFib(n));
	
	return 0;
}
