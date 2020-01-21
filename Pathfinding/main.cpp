#include <stdio.h>

int main(int argc, const char* argv[]) {
	if (argc != 3) {
		printf("Please run this program again, specifying an input file and an output file.");
		return 0;
	}
	printf("The input file is %s and the output file is %s", argv[1], argv[2]);
	return 0;
}