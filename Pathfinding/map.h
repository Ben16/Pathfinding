#include <json/json.h>
#include <string>
#include <vector>
#ifndef MAP
#define MAP

class Map {
public:
	Json::Reader reader;
	Json::Value val;
	Map(std::string jsonRaw);
	std::string findPath();
	std::vector<std::vector<int>> mapCosts;
};

#endif