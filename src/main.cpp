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

// opening connections data and loading into campusGraph object
void connectionsLoad(campusGraph& graph, const std::vector<std::string>& acceptedBuildings, const std::string& dataDir) {
	graph.loadFromFile(dataDir + "/connections.txt", acceptedBuildings);
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
	campusGraph graph; // Load the campus graph from the connections file
	buildingsLoad(dataDir, acceptedBuildings);
	facilitiesLoad(facilityTable, acceptedBuildings, dataDir);
	departmentsLoad(deptTree, dataDir);
	connectionsLoad(graph, acceptedBuildings, dataDir);

	int caseNumber = 0;
	while (caseNumber != 18) {
		std::cout << "===== Welcome to the Campus Navigation and Facility Analytics System! =====" << std::endl;
		std::cout << "Select an option from the menu below:" << std::endl;
		std::cout << "1. View all facilities\n"
			<< "2. Search facility by ID\n"
			<< "3. Search facility by name\n" 
			<< "4. Search facility by category\n"
			<< "5. View campus graph\n" 
			<< "6. Display graph counts and degrees\n"
			<< "7. Find reachable buildings (BFS)\n"
			<< "8. Find shortest path (BFS)\n"
			<< "9. Add department\n"
			<< "10. Search department\n"
			<< "11. Display department hierarchy*\n"
			<< "12. Preorder traversal**\n"
			<< "13. Postorder traversal\n"
			<< "14. Level-order traversal\n"
			<< "15. Department statistics\n"
			<< "16. Facility category report\n"
			<< "17. Graph connectivity report\n"
			<< "18. Exit\n";
		std::cout << "Enter your choice: ";

		if (!(std::cin >> caseNumber)) { // Get reponse, and clear anything that is not an int 
			std::cin.clear();
			std::string invalidInput;
			std::getline(std::cin, invalidInput);
			caseNumber = 0;
			std::cout << "====== Invalid option. Please enter a number from 1 to 18. ======" << std::endl;
			continue;
		}
		std::string remainingInput;
		std::getline(std::cin, remainingInput);  // if input is int, it passes through to this 
		
	switch(caseNumber)
		{
		case 1:
				// Display all facilities
				facilityTable.displayAll();
				break;
		case 2:
			{
				// search facility by ID
			std::cout << "Enter facility ID: ";
            std::string rawId;
            std::getline(std::cin, rawId);
            if (HashTable::trim(rawId).empty()) {
                std::cout << " ====== Error: Facility ID cannot be blank. ======";
            } else {
                facilityTable.searchAndPrintById(rawId);
            }
            break;
			}
		case 3:
		{
			// search facility by name
			std::cout << "Enter facility name: ";
			std::string rawName;
			std::getline(std::cin, rawName);
			facilityTable.searchAndPrintByName(rawName);
			break;
		}
		case 4:
		{
			// search facility by category
			std::cout << "Enter facility category: ";
			std::string rawCategory;
			std::getline(std::cin, rawCategory);
			facilityTable.searchAndPrintByCategory(rawCategory);
			break;
		}
		case 5:
			// View campus graph - Will Jenkins
			graph.displayGraph(acceptedBuildings); // Displays connections between buildings in the campus graph -- 
			// need to pull from buildings.txt and connections.txt!! Otherwise it will omit observatory (and maybe other buildings that don't have eonnections in testing).
			break;
		case 6:
			// Display graph counts and degrees
			graph.displayCAD(acceptedBuildings);
			break;
		case 7:
			// Find reachable buildings(BFS) -- promts for one building and then shows fastest route to all locations 
            {
				/* EXAMPLE 
				From Library:
				Library (distance 0)
				Student Union (distance 1)
				Business Building (distance 1)
				Science Building (distance 2)
				Gym (distance 2)
				Engineering Building (distance 3)
				Reachable: 6 of 7 buildings

				From Observatory:
				Observatory (distance 0)
				Reachable: 1 of 7 buildings*/

                std::cout << "===== Find Reachable Buildings (BFS) =====" << std::endl;
                std::cout << " - Available buildings:" << std::endl;
                for (const auto& building : acceptedBuildings) {
                    std::cout << building << '\n';
                } // Lists available buildings 
				std::cout << "\n";

                std::cout << "Enter the starting building: ";
				std::string userStart;
                std::getline(std::cin, userStart); // gets starting building from the user  
				std::cout << "------------------ \n";

				graph.reachableBFS(userStart, acceptedBuildings);
			}

			break;	
		case 8:
			// Find shortest path (BFS)
						{
		std::string userStart;
		std::string userEnd;
		std::string line;

		std::cout << "===== Find Shortest Path (BFS) =====" << std::endl;
		std::cout << " - Available buildings: " << std::endl;

		
		for (const auto& building : acceptedBuildings) {
    		std::cout << building << '\n';
		} // Lists all the buildings in the acceptedBuildings vector

			std::cout << "-------------------------------------------" << std::endl;
			std::cout << "What building would you like to start from? (Please enter the building name): ";
			std::getline(std::cin, userStart);
			std::cout << "What building would you like to end at? (Please enter the building name): ";
			std::getline(std::cin, userEnd);
			std::cout << "-------------------------------------------" << std::endl;
			graph.BFS(userStart, userEnd, acceptedBuildings);
		}
		break;
		case 9:
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
		case 10:
		{
			std::cout << "Enter department name to search: ";
			std::string targetDept;
			std::getline(std::cin, targetDept);
			if (targetDept.empty()) {
				std::cout << "====== Error: Department name cannot be blank. ======" << std::endl;
			} else {
				deptTree.searchDepartment(targetDept);
			}
			break;
		}
		case 11:
			deptTree.displayHierarchy(); // *Display the department tree with each department on a separate line, using four spaces of indentation per depth level.
			break;
		case 12:
			deptTree.displayPreorder(); // **Display the departments in preorder traversal order as a sequence, without indentation.
			break;
		case 13: 
			//  Postorder traversal 
			break;
		case 14:
			// Level-order traversal
			break;
		case 15: 
			// department statictis 
			break;
		case 16: 
			// Facility category report: for each distinct category, display facility count and percentage of all accepted facilities (one decimal place). Group category names ignoring capitalization and preserve the spelling of the first accepted category. Display the total count. If no facilities exist, state that none are available and show total zero; never divide by zero.
			facilityTable.displayCategoryReport();
			break;
		case 17:
			// Graph connectivity report: display total buildings, total connections, every building tied for highest degree, isolated buildings (or None), and the number of connected components. If there are no buildings, display a clear message with zero totals.
			// Graph connectivity report (Final) -- EXAMPLE
			// Total buildings: 
			// Total connections: 6
			// Highest degree (3): Student Union
			// Isolated buildings: Observatory
			// Connected components: 
			break;
		case 18:
			// Exit
			break;
		default:
			std::cout << "====== Invalid option. Please try again. ======" << std::endl;
			break;
		}
	}
	return 0;
}
