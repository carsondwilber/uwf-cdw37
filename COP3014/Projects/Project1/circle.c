#include <stdio.h>

const double pi = 3.14159265359;

int main(void) {
	double radius;
	
	printf("Enter the radius of your circle: ");
	scanf("%lf", &radius);
	
	double area = pi*radius*radius;
	double circumference = 2*pi*radius;
	
	printf("The area of a circle with radius %lf is %lf.\n", radius, area);
	printf("The circumference of a circle with radius %lf is %lf.\n", radius, circumference);	

	return 0;
}
