#include <stdio.h>
#include <fstream>
#include <string>
#include "map.h"

std::string getDirection(std::pair<int, int> tile1, std::pair<int, int> tile2) {
	if (tile2.first == tile1.first - 1) {
		return "left";
	}
	if (tile2.first == tile1.first + 1) {
		return "right";
	}
	if (tile2.second == tile1.second - 1) {
		return "down";
	}
	if (tile2.second == tile1.second + 1) {
		return "up";
	}
}

std::vector<std::pair<float, float>> smoothPaths(std::vector<std::pair<int, int>> originalPaths) {
	//existing coordinates refer to bottom left corner of tile
	bool left = true; // false for right
	bool bottom = true; // false for top
	std::string direction = "";
	std::vector<std::pair<float, float>> ret;
	if (originalPaths.size() < 2) {
		for (std::pair<int, int> tile : originalPaths) {
			ret.push_back(std::make_pair((float)tile.first, (float)tile.second));
		}
		return ret;
	}
	//first tile will be the same, since we assume starting in the bottom left corner
	auto startTile = originalPaths[0];
	ret.push_back(std::make_pair((float)startTile.first, (float)startTile.second));
	int convertedIndex = 0;
	int numWithoutTurn = 0;
	for (int i = 1; i < originalPaths.size(); ++i) {
		std::pair<int, int> tile = originalPaths[i];
		std::string newDirection = getDirection(originalPaths[i - 1], originalPaths[i]);
		if (direction == "") {
			//first tile
			direction = newDirection;
			if ((left && direction == "right") || (bottom && direction == "up") || (!left && direction == "left") || (!bottom && direction == "down")) {
				++numWithoutTurn; //cross an extra square at beginning
			}
		}
		else if (direction == newDirection) {
			++numWithoutTurn;
		}
		else {
			float deltaY = 0;
			float deltaX = 0;
			float shiftY = 0;
			float shiftX = 0;
			if (!left) {
				shiftX = 1; //right side of square
			}
			if (!bottom) {
				shiftY = 1; // top part of square
			}
			//change in direction

			//initial direction is right
			if (direction == "right") {
				if (newDirection == "up") {
					//the point we want is bottom left of the next (post-turn tile)
					//so we should evaluate the last "fixed" point we have
					deltaX += 0;
					if (bottom) {
						deltaY += 1.0 / (float)numWithoutTurn;
					}
					else {
						deltaY += 0;
					}
					bottom = true;
					left = true; // start at bottom left of tile after turn
				}
				if (newDirection == "down") {
					//the point we want is bottom left of the next (post-turn tile)
					//so we should evaluate the last "fixed" point we have
					deltaX += 0;
					if (bottom) {
						deltaY += 0;
					}
					else {
						deltaY += -1.0 / (float)numWithoutTurn;
					}
					bottom = false;
					left = true; // start at top left of tile after turn
				}
			}

			//initial direction is left
			if (direction == "left") {
				if (newDirection == "up") {
					//the point we want is bottom left of the next (post-turn tile)
					//so we should evaluate the last "fixed" point we have
					deltaX += 0;
					if (bottom) {
						deltaY += 1.0 / (float)numWithoutTurn;
					}
					else {
						deltaY += 0;
					}
					bottom = true;
					left = false; // start at bottom right of tile after turn
				}
				if (newDirection == "down") {
					//the point we want is bottom left of the next (post-turn tile)
					//so we should evaluate the last "fixed" point we have
					deltaX += 0;
					if (bottom) {
						deltaY += 0;
					}
					else {
						deltaY += -1.0 / (float)numWithoutTurn;
					}
					bottom = false;
					left = false; // start at top right of tile after turn
				}
			}

			//initial direction is up
			if (direction == "up") {
				if (newDirection == "right") {
					//the point we want is bottom left of the next (post-turn tile)
					//so we should evaluate the last "fixed" point we have
					deltaY += 0;
					if (left) {
						deltaX += 1.0 / (float)numWithoutTurn;
					}
					else {
						deltaX += 0;
					}
					bottom = true;
					left = true; // start at bottom left of tile after turn
				}
				if (newDirection == "left") {
					//the point we want is bottom left of the next (post-turn tile)
					//so we should evaluate the last "fixed" point we have
					deltaY += 0;
					if (left) {
						deltaX += 0;
					}
					else {
						deltaX += -1.0 / (float)numWithoutTurn;
					}
					bottom = true;
					left = false; // start at top left of tile after turn
				}
			}

			//initial direction is down
			if (direction == "down") {
				if (newDirection == "right") {
					//the point we want is bottom left of the next (post-turn tile)
					//so we should evaluate the last "fixed" point we have
					deltaY += 0;
					if (left) {
						deltaX += 1.0 / (float)numWithoutTurn;
					}
					else {
						deltaX += 0;
					}
					bottom = false;
					left = true; // start at bottom left of tile after turn
				}
				if (newDirection == "left") {
					//the point we want is bottom left of the next (post-turn tile)
					//so we should evaluate the last "fixed" point we have
					deltaY += 0;
					if (left) {
						deltaX += 0;
					}
					else {
						deltaX += -1.0 / (float)numWithoutTurn;
					}
					bottom = false;
					left = false; // start at top left of tile after turn
				}
			}

			for (int j = convertedIndex + 1; j <= convertedIndex + numWithoutTurn; ++j) {
				ret.push_back(std::make_pair((float)originalPaths[j].first + (deltaX * (j - convertedIndex)), (float)originalPaths[j].second + (deltaY * (j - convertedIndex))));
			}
			//we have made it to the corner. This corner will double count, essentially, so we need to "skip" one tile
			convertedIndex += numWithoutTurn + 1;
			//++i;
			numWithoutTurn = 0;
			direction = ""; //treat first tile after turn like new start
		}
	}

	//now we have everything but the last bit leading to the finish
	float deltaX = 0;
	float deltaY = 0;
	if (!left) {
		deltaX = 1.0;
	}
	if (!bottom) {
		deltaY = 1.0;
	}
	for (int i = convertedIndex + 1; i < originalPaths.size(); ++i) {
		ret.push_back(std::make_pair((float)originalPaths[i].first + deltaX, (float)originalPaths[i].second + deltaY));
	}

	return ret;
}

int main(int argc, const char* argv[]) {
	for (int i = 0; i < argc; ++i) {
		printf("%s\n", argv[i]);
	}
	if (argc < 3) {
		printf("Please run this program again, specifying an input file and an output file.");
		system("pause");
		return 0;
	}
	printf("The input file is %s and the output file is %s \n", argv[1], argv[2]);
	bool smooth = false;
	if (argc > 3) {
		//printf("argv3 is %s\n", argv[3]);
		//if (argv[3] == "-s") {
		printf("smoothing");
		smooth = true;
		//}
	}
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
	std::vector<std::pair<int, int>> output = map->findPath();
	printf("Start printing answer");

	if (smooth) {
		std::vector<std::pair<float, float>> floatOutput = smoothPaths(output);
		for (std::pair<float, float> tile : floatOutput) {
			printf("[%f,%f]\n", tile.first, tile.second);
		}
	}
	else {
		for (std::pair<int, int> tile : output) {
			printf("[%d,%d]\n", tile.first, tile.second);
		}
	}

	printf("End printing answer");
	delete map;
	system("pause");

	return 0;
}