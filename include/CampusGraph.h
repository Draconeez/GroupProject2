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
	unsigned int no_vertices_ = 0;
	unsigned int no_edges_ = 0;
	
public:
	explicit campusGraph();

	// Getting info from the txt files and normalizing it 
	static std::string toLower(const std::string& str);
	static std::string trim(const std::string& str);
	bool isCommentOrBlank(const std::string& line);
	static std::vector<std::string> splitLine(const std::string& line);
	static std::string normalizeBuildingName(const std::string& rawBldg);
	bool isAdjacent(const std::string& src, const std::string& dest) const; 
	void loadFromFile(const std::string& filename, const std::vector<std::string>& acceptedBuildings);

	// Setting up the graph 
	void addVertex(const std::string& vertex);
	void addEdge(const std::string& src, const std::string& dest);
	const std::unordered_map<std::string, std::vector<std::string>>& getAdjList() const;

	// Graph functions 
	void displayGraph(const std::vector<std::string>& acceptedBuildings);
	void reachableBFS(const std::string& startNode, const std::vector<std::string>& acceptedBuildings); //Option 7
	void BFS(const std::string& startNode, const std::string& endNode, const std::vector<std::string>& acceptedBuildings); 	// BFS
	void displayCAD(const std::vector<std::string>& acceptedBuildings);  	// Counts and degrees

};
#endif 