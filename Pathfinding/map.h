#include <json/json.h>
#include <string>
#ifndef MAP
#define MAP

class Map {
public:
	Json::Reader reader;
	Json::Value val;
	Map(std::string jsonRaw);
	std::string findPath();
};

#endif