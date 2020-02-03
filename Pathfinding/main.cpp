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
				//check not double counting
				float newX = (float)originalPaths[j].first + (deltaX * (j - convertedIndex) + shiftX);
				float newY = (float)originalPaths[j].second + (deltaY * (j - convertedIndex) + shiftY);
				std::pair<float, float> last = ret[ret.size() - 1];
				if (newX != last.first || newY != last.second) {
					ret.push_back(std::make_pair(newX, newY));
				}
			}
			//we have made it to the corner. This corner will double count, essentially, so we need to "skip" one tile
			convertedIndex += numWithoutTurn + 1;
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
		//check that we're not double counting a point
		float newX = (float)originalPaths[i].first + deltaX;
		float newY = (float)originalPaths[i].second + deltaY;
		std::pair<float, float> last = ret[ret.size() - 1];
		if (newX != last.first || newY != last.second) {
			ret.push_back(std::make_pair(newX, newY));
		}
	}

	return ret;
}

int main(int argc, const char* argv[]) {
	if (argc < 3) {
		printf("Usage: Pathfinding.exe inputfile.json outputfile.json [-s].");
		return 0;
	}
	bool smooth = false;
	if (argc > 3) {
		if (std::string(argv[3]) == "-s") {
			smooth = true;
		}
	}
	std::string outputFile = std::string(argv[2]);
	std::ifstream input;
	input.open(argv[1]);
	if (!input) {
		printf("The file %s does not exist.", argv[1]);
		return 0;
	}
	std::string contents = "";
	std::string line;
	while (input >> line) {
		contents = contents + line + "\n";
	}

	Map* map = new Map(contents);
	std::vector<std::pair<int, int>> output = map->findPath();

	FILE * out;
	fopen_s(&out, outputFile.c_str(), "w");

	fprintf(out, "[\n");

	if (smooth) {
		std::vector<std::pair<float, float>> floatOutput = smoothPaths(output);
		for (int i = 0; i < floatOutput.size(); ++i) {
			if (i < floatOutput.size() - 1) {
				fprintf(out, "[%f,%f],\n", floatOutput[i].first, floatOutput[i].second);
			}
			else {
				fprintf(out, "[%f,%f]\n", floatOutput[i].first, floatOutput[i].second);
			}
		}
	}
	else {
		for (int i = 0; i < output.size(); ++i) {
			if (i < output.size() - 1) {
				fprintf(out, "[%d,%d],\n", output[i].first, output[i].second);
			}
			else {
				fprintf(out, "[%d,%d]\n", output[i].first, output[i].second);
			}
		}
	}

	fprintf(out, "]");

	fclose(out);

	delete map;

	return 0;
}