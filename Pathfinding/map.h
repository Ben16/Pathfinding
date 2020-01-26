#include <json/json.h>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#ifndef MAP
#define MAP

class Map {
public:
	Json::Reader reader;
	Json::Value val;
	Map(std::string jsonRaw);
	std::vector<std::pair<int, int>> findPath();
	std::vector<std::vector<int>> mapCosts;
	std::pair<int, int> start;
	std::pair<int, int> dest;
private:
	int heuristic(std::pair<int, int> current, std::pair<int, int> dest);
	std::vector<std::pair<int, int>> getNeighbors(std::pair<int, int> current);
	int indexOf(std::vector<std::pair<int, int>> vec, std::pair<int, int> elem);
};

#endif