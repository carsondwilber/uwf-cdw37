#include <stdio.h>

enum JarType { Cup = 1, Pint, Quart, HalfGallon, FullGallon };
char* JarNames[5] = {"Cup", "Pint", "Quart", "Half Gallon", "Full Gallon"};
const double JarAmounts[5] = {236.588, 473.176, 946.353, 1892.705, 3785.41};

int main(void) {
	printf("# Type        Amount of liquid in ml");
	
	int x = 0;
	for (x; x < 5; ++x) {
		printf("\n%-2d%-12s%lf", x+1, JarNames[x], JarAmounts[x]);
	}
	
	enum JarType jarType = 0;
	unsigned int jarQuantity = 0;
	
	printf("\n");
		
	while (!(jarType == Cup || jarType == Pint || jarType == Quart || jarType == HalfGallon || jarType == FullGallon)) { 
		printf("\nEnter jar type: ");
		scanf("%u", &jarType);
	}
	while (jarQuantity == 0) {
		printf("\nEnter jar quantity: ");
		scanf("%u", &jarQuantity);
	}
	
	char* printName = "";
	
	switch (jarType) {
		default:
		case Cup:
			printName = JarNames[Cup-1];
			break;
		case Pint:
			printName = JarNames[Pint-1];
			break;
		case Quart:
			printName = JarNames[Quart-1];
			break;
		case HalfGallon:
			printName = JarNames[HalfGallon-1];
			break;
		case FullGallon:
			printName = JarNames[FullGallon-1];
			break;
	}

	printf("\nYou are ordering %d %ss for a total of %lf ml.\n", jarQuantity, printName, jarQuantity*JarAmounts[jarType-1]);	
	
	return 0;
}
