#include <json/json.h>
#include <string>
#include <vector>
#include <utility>
#ifndef MAP
#define MAP

class Map {
public:
	Json::Reader reader;
	Json::Value val;
	Map(std::string jsonRaw);
	std::string findPath();
	std::vector<std::vector<int>> mapCosts;
	std::pair<int, int> start;
	std::pair<int, int> dest;
};

#endif