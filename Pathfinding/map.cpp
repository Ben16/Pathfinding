#include "map.h"
#include "json/json.h"

Map::Map(std::string jsonRaw) {
	bool success = reader.parse(jsonRaw.c_str(), val);
	if (!success) {
		printf("failed to parse json");
		return;
	}

	//create map of costs
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
		mapCosts.insert(mapCosts.begin(), costs);

		//get source and dest
		Json::Value jsonStart = val["Start"];
		start = std::make_pair(jsonStart[0].asInt(), jsonStart[1].asInt());
		Json::Value jsonDest = val["Destination"];
		dest = std::make_pair(jsonDest[0].asInt(), jsonDest[1].asInt());
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

	printf("start at %d, %d\n", start.first, start.second);
	printf("dest at %d, %d\n", dest.first, dest.second);
	return "foo";
}