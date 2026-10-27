#include "CampusGraph.h"


campusGraph::campusGraph(bool directed) : isDirected(directed) {}

void campusGraph::addEdge(const std::string& src, const std::string& dest) {
	adjList[src].push_back(dest);
	if (!isDirected) {
		adjList[dest].push_back(src);
	}
}

bool campusGraph::loadFromFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		return false;
	}

	std::string line;
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string src, dest;
		if (std::getline(iss, src, '|') && std::getline(iss, dest, '|')) {
			addEdge(src, dest);
		}
	}
	file.close();
	return true;
}	

void campusGraph::displayGraph() const {

	std::cout << " ====== Connections between Campus Buildings ===== " << std::endl;
	for (const auto& pair : adjList) {
		std::cout << pair.first << "   -->   ";
		for (const auto& neighbor : pair.second) {
			std::cout << neighbor << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << "====================================================" << std::endl;
}

const std::unordered_map<std::string, std::vector<std::string>>& campusGraph::getAdjList() const {
	return adjList;
}