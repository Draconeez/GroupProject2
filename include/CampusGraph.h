#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>		
#include <sstream>

class campusGraph {

private:
	std::unordered_map<std::string, std::vector<std::string>> adjList;
	bool isDirected;

public:
	explicit campusGraph(bool directed = false);

	void addEdge(const std::string& src, const std::string& dest);

	bool loadFromFile(const std::string& filename);

	void displayGraph() const;

	const std::unordered_map<std::string, std::vector<std::string>>& getAdjList() const;
};

#endif 