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

int Map::heuristic(std::pair<int, int> current, std::pair<int, int> dest) {
	//use pythagorean theorem for as accurate a heuristic as possible
	int a = pow(dest.first - current.first, 2);
	int b = pow(dest.second - current.second, 2);
	int result = pow(a + b, 0.5f);
	return result;
	//manhattan distance
	//return abs(current.first - dest.first) + abs(current.second - dest.second);
}

int Map::indexOf(std::vector<std::pair<int, int>> vec, std::pair<int, int> elem) {
	for (int i = 0; i < vec.size(); ++i) {
		std::pair<int, int> e = vec[i];
		if (e.first == elem.first && e.second == elem.second) {
			return i;
		}
	}
	return -1;
}

std::vector<std::pair<int, int>> Map::getNeighbors(std::pair<int, int> current) {
	std::vector<std::pair<int, int>> ret;
	if (current.first > 0) {
		ret.push_back(std::make_pair(current.first - 1, current.second));
	}
	if (current.first < mapCosts[current.second].size() - 1) {
		ret.push_back(std::make_pair(current.first + 1, current.second));
	}
	if (current.second > 0) {
		ret.push_back(std::make_pair(current.first, current.second - 1));
	}
	if (current.second < mapCosts.size() - 1) {
		ret.push_back(std::make_pair(current.first, current.second + 1));
	}
	return ret;
}

std::vector<std::pair<int, int>> Map::findPath() {
	//copy map costs so you can edit it
	std::vector<std::vector<int>> costs(mapCosts);
	for (int y = 0; y < costs.size(); ++y) {
		for (int x = 0; x < costs[y].size(); ++x) {
			if (x == start.first && y == start.second) {
				continue; // leave start as its normal value
			}
			if (costs[y][x] != -1) {
				costs[y][x] = 0; // 0 means unreached, -1 means wall
			}
		}
	}

	//for debugging
	/*for (int i = 0; i < costs.size(); ++i) {
		for (int j = 0; j < costs[i].size(); ++j) {
			printf("%d", costs[i][j]);
		}
		printf("\n");
	}

	printf("start at %d, %d\n", start.first, start.second);
	printf("dest at %d, %d\n", dest.first, dest.second);*/

	std::map<std::pair<int, int>, std::pair<int, int>> previousTile;
	std::vector<std::pair<int, int>> toTraverse;
	std::vector<std::pair<int, int>> blacklist;
	toTraverse.push_back(start);

	//build the path
	while (toTraverse.size() > 0) {
		std::pair<int, int> traversing = toTraverse.back();
		toTraverse.pop_back();
		blacklist.push_back(traversing);
		if (traversing.first == dest.first && traversing.second == dest.second) {
			//we are at the end
			printf("Reached the end");
			break;
		}

		std::vector<std::pair<int, int>> neighbors = getNeighbors(traversing);
		for (std::pair<int, int> tile : neighbors) {
			int y = tile.second;
			int x = tile.first;
			if (costs[y][x] == -1) { // this is a wall, ignore it
				continue;
			}
			int newCost = mapCosts[y][x] + costs[traversing.second][traversing.first];
			if (costs[y][x] == 0 || costs[y][x] > newCost) {
				costs[y][x] = newCost;
				previousTile[tile] = traversing; 
				//printf("[%d,%d] leads to [%d,%d]\n", traversing.first, traversing.second, tile.first, tile.second);
				if (indexOf(blacklist, tile) != -1) {
					//continue;
				}
				int index = indexOf(toTraverse, tile);
				if (index > -1) {
					toTraverse.erase(toTraverse.begin() + index); // need to update priority
				}
				
				//add to the traverse list at the appropriate point
				bool added = false;
				int newHeuristic = heuristic(tile, dest) + costs[tile.second][tile.first];
				for (int i = 0; i < toTraverse.size(); ++i) {
					int y = toTraverse[i].second;
					int x = toTraverse[i].first;
					int currentVal = costs[y][x] + heuristic(toTraverse[i], dest);
					if (newHeuristic > currentVal) {
						//we want the lowest cost at the back, highest cost at the front
						toTraverse.insert(toTraverse.begin() + i, tile);
						added = true;
						break;
					}
				}

				if (!added) {
					//lowest cost, add to back
					toTraverse.push_back(tile);
				}
			}
		}
	}

	// print the final path
	if (previousTile.find(dest) == previousTile.end()) {
		return std::vector<std::pair<int, int>>();
	}
	std::vector<std::pair<int, int>> path;
	std::pair<int, int> current = dest;
	path.push_back(dest);
	while (previousTile.find(current) != previousTile.end()) {
		current = previousTile[current];
		path.push_back(current);
	}

	//debug to see final costs
	/*for (int i = 0; i < costs.size(); ++i) {
		for (int j = 0; j < costs[i].size(); ++j) {
			printf("[%d]", costs[i][j]);
		}
		printf("\n");
	}

	printf("start at %d, %d\n", start.first, start.second);
	printf("dest at %d, %d\n", dest.first, dest.second);*/

	return path;
}