#include <stdio.h>

int main(void) {
	float gpa, numberGrade, sum;
	int numCourses, i;
	char letterGrade, modifier;
	
	printf("Enter the number of courses: ");
	scanf("%d", &numCourses);
	
	for (i=0; i < numCourses; i++) {
		printf("What grade did you earn?\n");
		
		printf("Enter A, B, C, D, or F: ");
		scanf(" %c", &letterGrade);
		
		printf("Enter modifier - for letter grades A-, etc. + for letter grades B+, etc. and _ for letter grades A, B, etc.: ");
		scanf(" %c", &modifier);
		
		if (letterGrade == 'A')
			numberGrade = 4.0;
		else if (letterGrade == 'B')
			numberGrade = 3.0;
		else if (letterGrade == 'C')
			numberGrade = 2.0;
		else if (letterGrade == 'D')
			numberGrade = 1.0;
		else
			numberGrade = 0.0;
		
		if (modifier == '-') {
			numberGrade -= 0.3f;
		} else if (modifier == '+') {
			numberGrade += 0.3f;
		}
		
		sum += numberGrade;
	}
	
	gpa = sum/((float)numCourses);
	
	printf("Sum is %1.2f; numCourses is %d; The final gpa is %1.2f.\n", sum, numCourses, gpa);
	
	return 0;
}
