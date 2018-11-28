#include <stdio.h>
#include <math.h>

double pow (double base, double exponent);

int main(void) {
	unsigned int years;
	double rate, balance;
	
	printf("Enter the deposit: $");
	scanf("%lf", &balance);
	
	printf("Enter the interest rate (0-100): ");
	scanf("%lf", &rate);
	rate /= 100.0;
	
	printf("Enter the number of years: ");
	scanf("%u", &years);
	
	int i = 0;
	while (i < years) {
		++i;
		int x = 0;
		while (x < 12) {
			++x;
			balance *= 1.0+(1.0/12.0)*rate;
		}
	}
	
	printf("After %d number of years, the deposit in the savings account is now %.2lf\n", years, balance);
	
	return 0;
}
