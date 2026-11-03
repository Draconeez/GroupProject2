#include <iostream>
#include <fstream>		
#include <sstream>	
#include <vector>

#include "../include/HashTable.h"
#include "../include/DepartmentTree.h"
#include "../include/Facility.h"
#include "../include/CampusGraph.h"		

// Opening buildings data 
void buildingsLoad() {
	std::ifstream file("data/buildings.txt");
	if (!file.is_open()) {
		std::cerr << "Error opening buildings.txt" << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string buildingName;
		if (std::getline(iss, buildingName, ',')) {
			// Process the building name
		}
	}
	file.close();
}
// Opening connections data
void connectionsLoad() {
	std::ifstream file("data/connections.txt");
	if (!file.is_open()) {
		std::cerr << "Error opening connections.txt" << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string connections;
		if (std::getline(iss, connections, ',')) {
			// Process the connections
		}
	}
	file.close();
}

// Opening departments data
void departmentsLoad() {
	std::ifstream file("data/departments.txt");
	if (!file.is_open()) {
		std::cerr << "Error opening departments.txt" << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string departmentName;
		if (std::getline(iss, departmentName, ',')) {
			// Process the department name
		}
	}
	file.close();
}

// Opening facilities data
void facilitiesLoad(std::vector<Facility>& facilities) {
	std::ifstream file("data/facilities.txt");
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip line one, header
	if (!file.is_open()) {
		std::cerr << "Error opening facilities.txt" << std::endl;
		return;
	}
	std::string line;
	int linenumber = 0;
	while (std::getline(file, line)) {
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back(); 
		}
		linenumber++;
		int pos1 = line.find('|');
		int pos2 = line.find('|', pos1 + 1);
		int pos3 = line.find('|', pos2 + 1);

    std::string line;
    int linenumber = 0;
    int accepted = 0;
    int rejected = 0;

    while (std::getline(file, line)) {
        linenumber++;

        // Strip Windows carriage return (\r) and outer spaces
        std::string cleanedLine = HashTable::trim(line);

        // Skip blank lines and '#' comment lines
        if (cleanedLine.empty() || cleanedLine[0] == '#') {
            continue;
        }

        // Find the 3 '|' separators and make sure there isn't a 4th '|'
        size_t pos1 = cleanedLine.find('|');
        size_t pos2 = (pos1 != std::string::npos) ? cleanedLine.find('|', pos1 + 1) : std::string::npos;
        size_t pos3 = (pos2 != std::string::npos) ? cleanedLine.find('|', pos2 + 1) : std::string::npos;
        size_t pos4 = (pos3 != std::string::npos) ? cleanedLine.find('|', pos3 + 1) : std::string::npos;

        // Check 1: Must have exactly 3 '|' bars (4 fields)
        if (pos1 == std::string::npos || pos2 == std::string::npos ||
            pos3 == std::string::npos || pos4 != std::string::npos) {
            std::cout << "Warning [facilities.txt:" << linenumber
                      << "]: Wrong field count.\n";
            rejected++;
            continue;
        }

        std::string id = HashTable::normalizeId(cleanedLine.substr(0, pos1));
        std::string name = HashTable::trim(cleanedLine.substr(pos1 + 1, pos2 - pos1 - 1));
        std::string rawBuilding = HashTable::trim(cleanedLine.substr(pos2 + 1, pos3 - pos2 - 1));
        std::string category = HashTable::trim(cleanedLine.substr(pos3 + 1));

        // Check 2: No empty fields
        if (id.empty() || name.empty() || rawBuilding.empty() || category.empty()) {
            std::cout << "Warning [facilities.txt:" << linenumber
                      << "]: Empty required field.\n";
            rejected++;
            continue;
        }

        // Check 3: ID must be F followed by 3 digits
        if (!HashTable::isValidIdFormat(id)) {
            std::cout << "Warning [facilities.txt:" << linenumber
                      << "]: Invalid ID format '" << id << "'.\n";
            rejected++;
            continue;
        }

        // Check 4: Building must match an accepted building from buildings.txt
        std::string matchedBuilding = "";
        for (int i = 0; i < (int)acceptedBuildings.size(); i++) {
            if (HashTable::toLower(acceptedBuildings[i]) == HashTable::toLower(rawBuilding)) {
                matchedBuilding = acceptedBuildings[i];
                break;
            }
        }
        if (matchedBuilding.empty()) {
            std::cout << "Warning [facilities.txt:" << linenumber
                      << "]: Unknown building '" << rawBuilding << "'.\n";
            rejected++;
            continue;
        }

        // Check 5: Insert into HashTable (rejects duplicate IDs)
        Facility newFac(id, name, matchedBuilding, category);
        if (!facilityTable.insert(newFac)) {
            std::cout << "Warning [facilities.txt:" << linenumber
                      << "]: Duplicate facility ID '" << id << "'.\n";
            rejected++;
            continue;
        }
        accepted++;
    }
    file.close();
    std::cout << "facilities.txt: " << accepted << " accepted, "
              << rejected << " rejected (hash capacity "
              << facilityTable.getCapacity() << ", entries "
              << facilityTable.getEntryCount() << ", collisions "
              << facilityTable.getCollisionCount() << ")\n";
}

int main() {
	//Load resources from file
	HashTable facilityTable;
	std::vector<std::string> acceptedBuildings; // Populate this with the list of accepted buildings
	facilitiesLoad(facilityTable, acceptedBuildings);

	int caseNumber = 0;
	while (caseNumber != 10) {
		std::cout << "===== Welcome to the Campus Navigation and Facility Analytics System! =====" << std::endl;
		std::cout << "Select an option from the menu below:" << std::endl;
		std::cout << " 1. View all facilities\n"
			<< " 2. Search facility by ID\n"
			<< " 3. View campus graph\n"
			<< " 4. Display graph counts and degrees\n"
			<< " 5. Find reachable buildings(BFS)\n"
			<< " 6. Add department\n"
			<< " 7. Search department\n"
			<< " 8. Display department hierarchy\n"
			<< " 9. Run preorder traversal\n"
			<< " 10. Exit\n";
		std::cin >> caseNumber;

		switch (caseNumber)
		{
		case 1:
			facilityTable.displayAll();
			break;
		case 2:
			// Search facility by ID 
			break;
		case 3:
			// View campus graph - Will's Job
		{
			campusGraph graph(false); // Create an undirected graph

			if (graph.loadFromFile("data/connections.txt")) {
				graph.displayGraph();
			}
			else {
				std::cerr << "Error loading graph from file." << std::endl;
			}
		}
			break;
		case 4:
			// Display graph counts and degrees
			break;
		case 5:
			// Find reachable buildings(BFS) 
			break;
		case 6:
			// Add department
			break;
		case 7:
			// Search department 
			break;
		case 8:
			// Display department hierarchy
			break;
		case 9:
			// Run preorder traversal
			break;
		case 10:
			// Exit
			break;
		default:
			std::cout << "Invalid option. Please try again." << std::endl;
			break;
		}
	}
	return 0;
}