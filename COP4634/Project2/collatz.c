#include "collatz.h"

int collatz(int n)
{
	int a = n, k = 0;
	
	while (a > 1)
	{
		if (a % 2) a = 3*a + 1;
		else a /= 2;
		k++;
	}
	
	return k;
}
