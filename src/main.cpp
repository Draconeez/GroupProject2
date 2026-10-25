#include <iostream>
#include <fstream>		
#include <sstream>	
#include <vector>
#include "../include/CampusGraph.h"
#include "../include/DepartmentTree.h"
#include "../include/Facility.h"
#include "../include/HashTable.h"

// Opening buildings data 
void buildingsLoad(){
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
	void facilitiesLoad() {
		std::ifstream file("data/facilities.txt");
		if (!file.is_open()) {
			std::cerr << "Error opening facilities.txt" << std::endl;
			return;
		}
		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string facilityName;
			if (std::getline(iss, facilityName, ',')) {
				// Process the facility name
			}
		}
		file.close();
	}

	int main() {
		buildingsLoad();
		connectionsLoad();
		departmentsLoad();
		facilitiesLoad();

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
				// View all facilities
				break;
			case 2:
				// Search facility by ID
				break;
			case 3:
				// View campus graph
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
        std::cout << " === Exiting Program. === " << std::endl;
        return 0;
				break;
			default:
				std::cout << " === Invalid option. Please try again. === " << std::endl;
				break;
			}
		}
		return 0;
	}
