#include "map.h"
#include "json/json.h"

Map::Map(std::string jsonRaw) {
	bool success = reader.parse(jsonRaw.c_str(), val);
	if (!success) {
		printf("failed to parse json");
		return;
	}
	//Json::Value map = val["Map"];
	Json::Value map = val["Map"];
	if (map.empty()) {
		printf("no map specified");
		return;
	}
	int len = map.size();
	
	//Json::StyledWriter writer;
	Json::StreamWriterBuilder swb;
	for (int i = 0; i < len; ++i) {
		std::string line = Json::writeString(swb, map[i]);
		printf(line.c_str());
		printf("\n");
	}
}

std::string Map::findPath() {
	return "foo";
}