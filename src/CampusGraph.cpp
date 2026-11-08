//File: CampusGraph.cpp
//Purpose: Implements the campusGraph class - loads connections.txt, gathers the edges to create a graph, displays connections using an adjacency list. BFS uses connections for the shortest route. DisplayCAD for the degrees of each building.  
//Author: Will
#include "CampusGraph.h"

campusGraph::campusGraph(bool directed) : isDirected(directed) {}

//Converts a string to lowercase
std::string campusGraph::toLower(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}

namespace {
    std::string normalizeBuildingName(const std::string& name) {
        std::string normalized;
        bool inSpace = false;

        for (unsigned char ch : name) {
            if (std::isspace(ch)) {
                inSpace = true;
            } else {
                if (inSpace && !normalized.empty()) {
                    normalized.push_back(' ');
                }
                normalized.push_back(static_cast<char>(std::tolower(ch)));
                inSpace = false;
            }
        }
        return normalized;
    }
}

//Trims leading and trailing spaces, tabs, carriage returns, and newlines 
std::string campusGraph::trim(const std::string& str) {
    const std::string whitespace = " \t\r\n";
    size_t first = str.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(whitespace);
    return str.substr(first, last - first + 1);
}

// Returns true if a line is blank or starts with '#'
bool campusGraph::isCommentOrBlank(const std::string& line) {
    std::string cleaned = trim(line);
    if (cleaned.empty()) {
        return true;
    }
    if (cleaned[0] == '#') {
        return true;
    }
    return false;
}

void campusGraph::loadFromFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening " << filename << std::endl; // added error message for clarity
		return;
	}			

	std::string line;

	while (std::getline(file, line)) {

		if (isCommentOrBlank(line)) {
        continue;
    }
	
		std::istringstream iss(line);
		std::string src, dest;
		if (std::getline(iss, src, '|') && std::getline(iss, dest, '|')) {
			addEdge(toLower(src), toLower(dest));
		}
	}
	file.close();
	return;
}	

void campusGraph::addEdge(const std::string& src, const std::string& dest) {
	std::string normalizedSrc = normalizeBuildingName(src);
	std::string normalizedDest = normalizeBuildingName(dest);
	adjList[normalizedSrc].push_back(normalizedDest);
	if (!isDirected) {
		adjList[normalizedDest].push_back(normalizedSrc);
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
	const std::string normalizedStart = normalizeBuildingName(startNode);
	const std::string normalizedEnd = normalizeBuildingName(endNode);

	if (adjList.find(normalizedStart) == adjList.end()) {
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
	visited.insert(normalizedStart);
	q.push(normalizedStart);
	bool found = false;

	std::cout << "You are located in " << normalizedStart << " and trying to get to " << normalizedEnd << "\n";
	std::cout << " ------------------------------------------------\n";

	while (!q.empty()) {
		std::string currentNode = q.front();
		q.pop();
		if (currentNode == normalizedEnd) { // if destination is found, it ends 
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
		std::cout << "Path found from " << normalizedStart << " to " << normalizedEnd << ":\n";

		// Reconstruct  path backwards
		std::vector<std::string> path;
		std::string current = normalizedEnd;
		while (current != normalizedStart) {
			path.push_back(current);
			current = parent[current];
		}
		path.push_back(normalizedStart);

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
		std::cout << "No path exists between " << normalizedStart << " and " << normalizedEnd << ".\n";
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
