#include "map.h"
#include "json/json.h"

Map::Map(std::string jsonRaw) {
	bool success = reader.parse(jsonRaw.c_str(), val);
	if (!success) {
		printf("failed to parse json");
		return;
	}

	Json::Value map = val["Map"];
	if (map.empty()) {
		printf("no map specified");
		return;
	}
	int len = map.size();
	
	Json::StreamWriterBuilder swb;
	for (int i = 0; i < len; ++i) {
		std::string line = Json::writeString(swb, map[i]);
		const char * line_chars = line.c_str();
		std::vector<int> costs;
		for (int j = 0; j < line.size(); ++j) {
			char current = line_chars[j];
			if (current == 'X') {
				costs.push_back(-1);
			}
			else {
				int val = (int)current - 48;
				if (val > 0 && val <= 9) {
					costs.push_back(val);
				}
			}
		}
		mapCosts.push_back(costs);
	}
}

std::string Map::findPath() {
	//for debugging
	for (int i = 0; i < mapCosts.size(); ++i) {
		for (int j = 0; j < mapCosts[i].size(); ++j) {
			printf("%d", mapCosts[i][j]);
		}
		printf("\n");
	}
	return "foo";
}