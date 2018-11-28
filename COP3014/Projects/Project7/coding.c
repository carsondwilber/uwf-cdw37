#include <stdio.h>

typedef struct {
	char from[28];
	char to[28];
} CodeT;

void encodeStr ( char *input, char *output, CodeT code ) {
	int i, c;
	for (i = 0; input[i] != '\0'; ++i) {
		c = 0;
		while (code.from[c] != input[i]) { ++c; }
		output[i] = code.to[c];
	}
	output[i] = '\0';
}

int main(void) {
	CodeT code = { .from = " abcdefghijklmnopqrstuvwxyz",
			.to  = ".172093%@#+!:_-$^*()854=6?>" };
	
	char input[30] = {'\0'};
	char output[30] = {'\0'};
	printf("Line to encode: ");
	scanf("%s", &input[0]);
	while (input[0] != '\0') {
		encodeStr(&input[0], &output[0], code);
		printf("Encoded string: %s\n", &output);
		printf("Line to encode: ");
		scanf("%s", &input[0]);
	}
	
	return 0;
}
