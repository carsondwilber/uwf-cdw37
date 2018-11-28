#include <stdio.h>

int main (int argc, char** argv) {
	char c;
	int d;
	float f;
	
	c = 'a';
	d = 108;
	f = 300.2;
	
	printf("The value of c is %c.\n", c);
	printf("The value of c is %d.\n", c);
	printf("\n");
	printf("The value of d is %d.\n", d);
	printf("The value of d is %c.\n", d);
	printf("\n");
	printf("The value of f is %f.\n", f);
	printf("The value of f is %e.\n", f);
	
	return 0;
}
