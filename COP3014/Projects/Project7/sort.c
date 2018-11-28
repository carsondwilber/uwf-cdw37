#include <stdio.h>

typedef struct {
	char label[20];
} WordT;

void sort ( WordT *words, int numWords ) {
	int sorted[numWords];
	int i;
	int s = 0;
	char search[] = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
	int c;
	for (c = 0; c < 52; ++c) {
		for (i = 0; i < numWords; ++i) {
			if (words[i].label[0] == search[c]) {
				sorted[s++] = i;
			}
		}
	}
	for (i = 0; i < numWords; ++i) {
		int found = -1;
		for (c = 0; c < numWords; ++c) {
			if (sorted[c] == i) {
				found = c;
			}
		}
		if (found == -1) {
			sorted[s++] = i;
		}
	}
	for (i = 0; i < numWords; ++i) {
		printf("%s\n", words[sorted[i]].label);
	}
}

int main(void) {
	int num = 0;
	printf("Number of words to sort: ");
	scanf("%d", &num);
	
	WordT words[num];
	
	int i;
	for (i = 0; i < num; ++i) {
		printf("Word: ");
		scanf("%s", &words[i].label);
	}
	
	sort(&words[0], num);
	
	return 0;
}
