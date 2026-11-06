#include <iostream>
#include <fstream>		
#include <sstream>	
#include <vector>

#include "../include/HashTable.h"
#include "../include/DepartmentTree.h"
#include "../include/Facility.h"
#include "../include/CampusGraph.h"		

// Opening buildings data and loading into acceptedBuildings vector
void buildingsLoad(const std::string& dataDir, std::vector<std::string>& acceptedBuildings) {
	std::ifstream file(dataDir + "/buildings.txt");
	if (!file.is_open()) {
		std::cerr << "Error opening buildings.txt" << std::endl;
		std::cout << "buildings.txt: 0 accepted, 0 rejected" << std::endl;
		return;
	}
	std::string line;
	int lineNumber = 0;
    int accepted = 0;
    int rejected = 0;

	while (std::getline(file, line)) {
		lineNumber++;
        if (HashTable::isCommentOrBlank(line)) {
            continue;
        }
		std::vector<std::string> fields = HashTable::splitPipeLine(line);
        if (fields.size() != 1 || fields[0].empty()) 
		{
            std::cout << "Warning [buildings.txt:" << lineNumber<< "]: Wrong field count or empty building name." << std::endl;
            rejected++;
            continue;
        }
		
		std::string buildingName;
		std::string bldgName = fields[0];
        bool isDuplicate = false;
        for (int i = 0; i < (int)acceptedBuildings.size(); i++) 
		{
            if (HashTable::toLower(acceptedBuildings[i]) == HashTable::toLower(bldgName)) 
			{
                isDuplicate = true;
                break;
            }
        }

        if (isDuplicate) {
            std::cout << "Warning [buildings.txt:" << lineNumber<< "]: Duplicate building '" << bldgName << "'."<<std::endl;
            rejected++;
            continue;
        }

        acceptedBuildings.push_back(bldgName);
        accepted++;
	}
	file.close();
	std::cout << "buildings.txt: " << accepted << " accepted, " << rejected << " rejected"<< std::endl;
}
// Opening connections data
void connectionsLoad(const std::string& dataDir) {
	std::ifstream file(dataDir + "/connections.txt");
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
void departmentsLoad(const std::string& dataDir) {
	std::ifstream file(dataDir + "/departments.txt");
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
void facilitiesLoad(HashTable& facilityTable, const std::vector<std::string>& acceptedBuildings, const std::string& dataDir) {
    facilityTable.loadFromFile(dataDir + "/facilities.txt", acceptedBuildings);
}

int main(int argc, char* argv[]) {
	std::string dataDir = "data";
	if (argc > 1) 
	{
		dataDir = argv[1];
	}
	//Load resources from file
	HashTable facilityTable;
	std::vector<std::string> acceptedBuildings; // Populate this with the list of accepted buildings
	buildingsLoad(dataDir, acceptedBuildings);
	facilitiesLoad(facilityTable, acceptedBuildings, dataDir);

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
				// Display all facilities
				facilityTable.displayAll();
				break;
		case 2:
				// Search facility by ID
				std::cout << "Enter facility ID: ";
			{
			std::cout << "Enter facility ID: ";
            std::string rawId;
            std::getline(std::cin, rawId);
            if (HashTable::trim(rawId).empty()) {
                std::cout << "Error: Facility ID cannot be blank.";
            } else {
                facilityTable.searchAndPrintById(rawId);
            }
            break;
			}
		case 3:
			// View campus graph - Will's Job
		{
			campusGraph graph(false); // Create an undirected graph

			if (graph.loadFromFile(dataDir + "/connections.txt")) {
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