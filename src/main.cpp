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

// Opening departments data
void departmentsLoad(DepartmentTree& deptTree, const std::string& dataDir) 
{
	deptTree.loadFromFile(dataDir + "/departments.txt");
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
	HashTable facilityTable; // Load the facilities data from the file
	DepartmentTree deptTree; // Load the department hierarchy from the file
	std::vector<std::string> acceptedBuildings; // Populate this with the list of accepted buildings
	buildingsLoad(dataDir, acceptedBuildings);
	facilitiesLoad(facilityTable, acceptedBuildings, dataDir);
	departmentsLoad(deptTree, dataDir);
	// Campus graph and other data structures are now loaded and ready for use
	// Connections was loaded elsewhere
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
			{
			campusGraph graph(false); // setup as undirected (formality ig) 

			if (graph.loadFromFile(dataDir + "/connections.txt")) {
				graph.displayCAD();
			}
			else {
				std::cerr << "Error loading graph from file." << std::endl;
			}
		}
			break;
		case 5:
			// Find reachable buildings(BFS) 
		{
	std::string userStart;
	std::string userEnd;
	std::string line;

while (true) {
	std::cout << "What building would you like to start from? (Please enter the building name): ";
	std::cin.ignore();
	std::getline(std::cin, userStart);
	std::cout << "What building would you like to end at? (Please enter the building name): ";
	std::getline(std::cin, userEnd);

	buildingsLoad(dataDir, acceptedBuildings); // list buildings for user
	for (const auto& building : acceptedBuildings) {
		std::cout << building << std::endl;
	}

	campusGraph graph(false);
	if (graph.loadFromFile(dataDir + "/connections.txt")) {
		const auto& adjList = graph.getAdjList();
		if (adjList.find(userStart) != adjList.end()) {
			graph.BFS(userStart, userEnd);
			break; // Exit the loop if a valid building is found
		}
		else {
			std::cout << "Building not found. Please try again." << std::endl;
			break; // Exit the loop if the building is not found
		}
	}
	else {
		std::cerr << "Error loading graph from file." << std::endl;
		break; // Exit the loop if there's an error loading the graph
		}
	}
}
			break;
		case 6:
			{
			// Add department
			std::string deptName;
			std::cout << "Enter department name: ";
			std::getline(std::cin, deptName);

			if (deptTree.isEmpty()) {
				deptTree.addDepartment(deptName, "NONE");
			} else {
				std::string parentName;
				std::cout << "Enter parent department name: ";
				std::getline(std::cin, parentName);
				deptTree.addDepartment(deptName, parentName);
			}
			break;
		}
		case 7:
		{
			std::cout << "Enter department name to search: ";
			std::string targetDept;
			std::getline(std::cin, targetDept);
			if (targetDept.empty()) {
				std::cout << "Error: Department name cannot be blank." << std::endl;
			} else {
				deptTree.searchDepartment(targetDept);
			}
			break;
		}
	
		case 8:
			deptTree.displayHierarchy();
			break;
		case 9:
		deptTree.displayPreorder();
		
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
