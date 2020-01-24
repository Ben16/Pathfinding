#include <stdio.h>
#include <fstream>
#include <string>
#include "map.h"

int main(int argc, const char* argv[]) {
	if (argc != 3) {
		printf("Please run this program again, specifying an input file and an output file.");
		system("pause");
		return 0;
	}
	printf("The input file is %s and the output file is %s \n", argv[1], argv[2]);
	std::ifstream input;
	input.open(argv[1]);
	if (!input) {
		printf("The file %s does not exist.", argv[1]);
	}
	std::string contents = "";
	std::string line;
	while (input >> line) {
		contents = contents + line + "\n";
	}

	Map* map = new Map(contents);
	map->findPath();
	delete map;
	system("pause");

	return 0;
}