#include <stdio.h>

int main(void) {
	
	char character;
	int charcode;
	
	printf("Enter your character: ");
	scanf("%c", &character);
	
	charcode = (int) character;
	
	if (charcode >= (int) 'a' && charcode <= (int) 'z') {
		printf("You entered an 'a-z' character.\n");
	} else if (charcode >= (int) 'A' && charcode <= (int) 'Z') {
		printf("You entered an 'A-Z' character.\n");
	} else if (charcode >= (int) '0' && charcode <= (int) '9') {
		printf("You entered a '0-9' character.\n");
	} else {
		printf("You entered a special character.\n");
	}
	
	return 0;
}
