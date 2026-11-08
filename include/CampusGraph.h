//File: CampusGraph.h
//Purpose: Declare the campusGraph class - Sets the groundwork for an adjacency list map and BFS system
//Author: Will

#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>		
#include <sstream>
#include <string>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <limits>

class campusGraph {

private:
	std::unordered_map<std::string, std::vector<std::string>> adjList;
	bool isDirected;

public:
	explicit campusGraph(bool directed = false);

	static std::string toLower(const std::string& str);

	static std::string trim(const std::string& str);

	bool isCommentOrBlank(const std::string& line);

	void addEdge(const std::string& src, const std::string& dest);

	void loadFromFile(const std::string& filename);

	void displayGraph() const;

	const std::unordered_map<std::string, std::vector<std::string>>& getAdjList() const;

	// BFS
	void BFS(const std::string& startNode, const std::string& endNode);

	// Counts and degrees
	void displayCAD() const;

};

#endif 
