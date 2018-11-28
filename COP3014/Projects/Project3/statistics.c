#include <stdio.h>

int main(void) {
	int lowest = 120, highest = 0, age = 0, total = 0, counted = 0;
	
	while (age >= 0) {
		printf("Enter an age: ");
		scanf("%d", &age);
		if (age >= 0) {
			if (age < lowest) {
				lowest = age;
			}
			if (age > highest) {
				highest = age;
			}
			total += age;
			counted++;
		}
	}
	
	printf("The smallest entered age is %d.\n", lowest);
	printf("The largest entered age is %d.\n", highest);
	printf("The average age is %.2f.\n", ((float)total)/((float)counted));
	
	return 0;
}
