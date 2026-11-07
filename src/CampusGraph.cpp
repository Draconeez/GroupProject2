#include "CampusGraph.h"
#include <iostream>
#include <limits>

campusGraph::campusGraph(bool directed) : isDirected(directed) {}

bool campusGraph::loadFromFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening " << filename << std::endl; // added error message for clarity
		return false;
	}

	std::string line;
	std::string dummyHeader;
	std::getline(file, dummyHeader); // file.ignore() causing issues -- dummy header preffered to skip line one 

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

void campusGraph::addEdge(const std::string& src, const std::string& dest) {
	adjList[src].push_back(dest);
	if (!isDirected) {
		adjList[dest].push_back(src);
	}
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

// BFS searching for reachable buildings

void campusGraph::BFS(const std::string& startNode, const std::string& endNode)
{

	if (adjList.find(startNode) == adjList.end()) {
		std::cout << "Error: Starting node " << startNode << " does not exist.\n";
		return;
	}

	// Track visited buildings to avoid infinite loopinh
	std::unordered_set<std::string> visited;
	// Queue to manage the BFS 
	std::queue<std::string> q;
	// To create the actual path directions for the user     
	std::unordered_map<std::string, std::string> parent;



	// We start the BFS with the starting building the user entered 
	visited.insert(startNode);
	q.push(startNode);
	bool found = false;

	std::cout << "You are located in " << startNode << " and trying to get to " << endNode << "\n";
	std::cout << " ------------------------------------------------\n";

	while (!q.empty()) {
		std::string currentNode = q.front();
		q.pop();
		if (currentNode == endNode) {
			found = true;
			break;
		}

		auto it = adjList.find(currentNode);
		if (it == adjList.end()) {
			continue;
		}

		for (const std::string& neighbor : it->second) {
			if (visited.find(neighbor) == visited.end()) {
				visited.insert(neighbor);
				parent[neighbor] = currentNode; // Track how we got to this neighbor
				q.push(neighbor);
			}
		}
	}

	// Print the path for the user !
	if (found) {
		std::cout << "Path found from " << startNode << " to " << endNode << ":\n";

		// Reconstruct  path backwards
		std::vector<std::string> path;
		std::string current = endNode;
		while (current != startNode) {
			path.push_back(current);
			current = parent[current];
		}
		path.push_back(startNode);

		// Print path in correct order
		for (auto it = path.rbegin(); it != path.rend(); ++it) {
			std::cout << *it;
			if (it + 1 != path.rend()) {
				std::cout << " -> ";
			}
		}
		std::cout << "\n";
	}
	else {
		std::cout << "No path exists between " << startNode << " and " << endNode << ".\n";
	}
};

// Display Counts and Degrees of each vertex(building)!! ! 

void campusGraph::displayCAD() const 
{
	std::cout << " ====== Counts and Degrees ====== " << std::endl;

	// count - how many vertexs/buildings there are total
	size_t totalVertices = adjList.size();
	std::cout << "Total amount of buildings: " << totalVertices << std::endl;
	std::cout << "-------------------------------------------" << std::endl;

	// degrees - how many edges/connections there are between all the buildings 

	for (const auto& pair : adjList) {
		std::cout << " - " << pair.first << " is connected to " << pair.second.size() << " other building(s). " << std::endl;
	};
	std::cout << "-------------------------------------------" << std::endl;
};