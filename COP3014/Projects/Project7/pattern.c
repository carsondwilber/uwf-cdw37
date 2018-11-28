#include <stdio.h>

int contains(const char *text, const char *pattern) {
	int ret = -1;
	int start = -1;
	int current = 0;
	int i = 0;
	while (text[i] != '\0' && pattern[current] != '\0') {
		if (text[i] == pattern[current]) {
			if (start == -1) {
				start = i;
			}
			++current;
		} else {
			if (start != -1) {
				i = start;
				start = -1;
				current = 0;
			}
		}
		++i;
	}
	if (pattern[current] == '\0') {
		return start;
	} else {
		return -1;
	}
}

int main(void) {
	const char* text = "Hello, World";
	const char* text2 = "lowercase abbaba";
	const char* find[5] = {"lo", "orld", "Hello", "lower", "bab"};
	int i;
	
	for (i = 0; i < 5; ++i) {
		printf("Pattern %s occurs in %s at the location %d.\n", find[i], text, contains(text, find[i]));
	}
	
	for (i = 0; i < 5; ++i) {
		printf("Pattern %s occurs in %s at the location %d.\n", find[i], text2, contains(text2, find[i]));
	}
	
	printf("(Now your turn to input!)\n");
	
	char bufferFind[30] = {'\0'};
	char bufferText[50] = {'\0'};
	printf("Pattern ");
	scanf("%s", &bufferFind);
	printf(" occurs in ");
	scanf("%s", &bufferText);
	printf(" at location %d.\n", contains(&bufferText[0], &bufferFind[0]));
	
	return 0;
}
