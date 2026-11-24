//File: CampusGraph.cpp
//Purpose: Implements the campusGraph class - loads connections.txt, gathers the edges to create a graph, displays connections using an adjacency list. BFS uses connections for the shortest route. DisplayCAD for the degrees of each building.  
//Author: Will Jenkins -- Member B.
#include "CampusGraph.h"


// Constructor -- undirected graph anyways, not needed  
campusGraph::campusGraph(){}

//Trims excess spaces
std::string campusGraph::trim(const std::string& str) {
    const std::string whitespace = " \t\r\n";
    size_t first = str.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(whitespace);
    return str.substr(first, last - first + 1);
}

//Converts a string to lowercase
std::string campusGraph::toLower(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}

//Trims outer whitespace anc converts a building name to lowercase for normalization
std::string campusGraph::normalizeBuildingName(const std::string& rawBldg) {
    return toLower(trim(rawBldg));
}

// Splits a line into different fields wherever there is a '|'  
std::vector<std::string> campusGraph::splitLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string currentFields = "";

    for (int i = 0; i < (int)line.length(); i++) {
        if (line[i] == '|') {
            fields.push_back(trim(currentFields));
            currentFields = "";
        } else {
            currentFields += line[i];
        }
    }

    // Add the last field after the final '|'
    fields.push_back(trim(currentFields));
    return fields;
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

// Dtermines if edge exists already 
bool campusGraph::isAdjacent(const std::string& src, const std::string& dest) const
{
	// Check if the src vertex exists in the graph already
    auto it = adjList.find(src);
    if (it == adjList.end()) {
        return false;
    }

    // 2. Search the src's neighbors for the dest vertex
    const auto& neighbors = it->second;
    for (const std::string& neighbor : neighbors) {
        if (neighbor == dest) {
            return true;
        }
    }

    return false;
}

// loads connections.txt
void campusGraph::loadFromFile(const std::string& filename, 
							const std::vector<std::string>& acceptedBuildings) {
	
	std::string displayFilename = "connections.txt";

	// catches error if the file doesn't open -- won't crash 
	std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open " << displayFilename
                  << ". Treating file as empty.\n";
        std::cout << displayFilename << ": 0 accepted, 0 rejected \n";
        return;
    }			
	
	int accepted = 0;
	int rejected = 0;
	int lineNumber = 0;
	std::string line;

	while (std::getline(file, line)) {
		lineNumber++;

		if (isCommentOrBlank(line)) {
        continue;
    	}

		// Check 1 : Must have exactly 2 fields separated by '|' -- (Building A | Building B)
		std::vector<std::string> fields = splitLine(line);
		if (fields.size() != 2) {
			std::cout << "Warning [" << displayFilename << ":" << lineNumber << 
			"]: Line does not contain exactly one '|' separator." << std::endl;
			rejected++;
			continue;
		}

		std::string BuildingA = fields[0];
		std::string BuildingB = fields[1];

		// check #2 : Both src and dest must be non-empty
		    if (BuildingA.empty() || BuildingB.empty()) {
            std::cout << "Warning [" << displayFilename << ":" << lineNumber
                      << "]: Empty required field.\n";
            rejected++;
            continue;
        }

		// check #3 : Both src and dest must be in acceptedBuildings (buildings.txt)
        std::string targetSrc = normalizeBuildingName(BuildingA);
        std::string targetDest = normalizeBuildingName(BuildingB);
        std::string actualSrc = "";
        std::string actualDest = "";

        for (size_t i = 0; i < acceptedBuildings.size(); i++) {
            std::string normalizedAccepted = normalizeBuildingName(acceptedBuildings[i]);
            if (normalizedAccepted == targetSrc) {
                actualSrc = acceptedBuildings[i];
            }
            if (normalizedAccepted == targetDest) {
                actualDest = acceptedBuildings[i];
            }
        }
			// Rejects if either building wasn't in acceptedBuildings
			if (actualSrc.empty() || actualDest.empty()) {
         		 std::cout << "Warning [" << displayFilename << ":" << lineNumber
                    << "]: Unknown building reference '" << BuildingA << "' or '" << BuildingB << "'.\n";
            rejected++;
            continue;
			}

        // Check #4: No self-loops
        if (actualSrc == actualDest) {
            std::cout << "Warning [" << displayFilename << ":" << lineNumber << "]: Source and destination buildings are the same (" << actualSrc << ").\n";
            rejected++;
            continue;
        }

		// check #5 : no repeats 
		bool edgeExists = isAdjacent(targetSrc, targetDest) || isAdjacent(targetDest, targetSrc);
		if (edgeExists) { 
    		std::cout << "Warning [" << displayFilename << ":" << lineNumber 
              << "]: Duplicate edge between '" << actualSrc 
              << "' and '" << actualDest << "'.\n";
    		rejected++;
    		continue;
}
		// check #6 : insert the edge into the graph (addVertex and addEdge)
			for (const auto& building : acceptedBuildings) {
				addVertex(building);
			}
        	addEdge(BuildingA, BuildingB);
        accepted++;
	}
	file.close();

 std::cout << displayFilename << ": " 
 << accepted << " accepted, " 
 << rejected << " rejected \n";
}


// Establishing vertexes in Buildings.txt 
void campusGraph::addVertex(const std::string& vertex) {
	std::string normalizedVertex = normalizeBuildingName(vertex);
	if (adjList.find(normalizedVertex) == adjList.end()) {
		adjList[normalizedVertex] = std::vector<std::string>();
		no_vertices_++;
	}
}

// connections between buildings 
void campusGraph::addEdge(const std::string& src, const std::string& dest) {
	std::string normalizedSrc = normalizeBuildingName(src);
	std::string normalizedDest = normalizeBuildingName(dest);
	adjList[normalizedSrc].push_back(normalizedDest); 
	adjList[normalizedDest].push_back(normalizedSrc);

	no_edges_++;
}

void campusGraph::displayGraph(const std::vector<std::string>& acceptedBuildings) {

	std::cout << " ====== Connections between Campus Buildings ===== " << std::endl;
	for (const auto& building : acceptedBuildings) {

		std::string normalizedBldg = toLower(building); // normalises building

		std::cout << building << "   -->   "; // Prints accepted building first 

		auto it = adjList.find(normalizedBldg); // find building in adj list 
		if (it != adjList.end()) {
			// Iterate only through this building's connections
			for (const auto& neighbor : it->second) { 
				std::cout << neighbor << ", ";
			}
			std::cout << "\n";
		} else {
			std::cout << "No connections found \n";
		}
	}
	std::cout << "====================================================" << std::endl;
}

const std::unordered_map<std::string, std::vector<std::string>>& campusGraph::getAdjList() const {
	return adjList;
}

void campusGraph::reachableBFS(const std::string& startNode, const std::vector<std::string>& acceptedBuildings)
{
	const std::string normalizedStart = normalizeBuildingName(startNode);

	for (const auto& neighbor : acceptedBuildings) {
		BFS(startNode, neighbor, acceptedBuildings);
	}
	std::cout << "\n";
}


// BFS searching for reachable buildings
void campusGraph::BFS(const std::string& startNode, const std::string& endNode, const std::vector<std::string>& acceptedBuildings)
{
	const std::string normalizedStart = normalizeBuildingName(startNode);
	const std::string normalizedEnd = normalizeBuildingName(endNode);

	if (adjList.find(normalizedStart) == adjList.end()) { // checks start node 
		std::cout << normalizedStart << " is not a valid building. Try again. \n";
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

	std::cout << " -----------------------------------------------------------------------------------\n";
	std::cout << "You are located in the " << normalizedStart << " and trying to get to " << normalizedEnd << "\n";

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
		std::cout << "Path found from " << normalizedStart << " to " << normalizedEnd << ":   ";

		// Reconstruct  path backwards
		std::vector<std::string> path;
		std::string curr = normalizedEnd;
		while (curr != normalizedStart) {
			path.push_back(curr);
			curr = parent[curr];
		}
		path.push_back(normalizedStart); // Add starting node

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
		std::cout << " === No path exists between " << normalizedStart << " and " << normalizedEnd << " === \n";
	}
};



// Display Counts and Degrees of each vertex(building)!! ! 
void campusGraph::displayCAD(const std::vector<std::string>& acceptedBuildings)
{
	std::cout << " ======= Counts and Degrees ======= " << std::endl;

	// count - total Vertices/buildings
	size_t totalVertices = acceptedBuildings.size();
	std::cout << "Total amount of buildings: " << totalVertices << std::endl;
	std::cout << "-------------------------------------------" << std::endl;

	size_t totalDegreeSum = 0;
	// degrees - how many edges/connections there are between all the buildings 
	for (const auto& building : acceptedBuildings) {
		size_t degree = 0;

		std::string normalizedBldg = toLower(building); 

		auto it = adjList.find(normalizedBldg);
		if (it != adjList.end()) {
			degree = it->second.size();
		}

		totalDegreeSum += degree;
		std::cout << " - " << building << " is connected to " << degree << " other building(s)." << std::endl;
	};
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "Total connections (edges): " << (totalDegreeSum / 2) << std::endl; 
	std::cout << "================================================" << std::endl;
};
