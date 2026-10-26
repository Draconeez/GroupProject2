#include "Facility.h"
//Constructors
Facility::Facility()
{
    facilityID = "";
    facilityName  = "";
    facilityBuilding = "";
    facilityCategory   = "";
};
Facility::Facility(std::string id, std::string name, std::string building, std::string category)
{
    facilityID  = id;
    facilityBuilding = building;
    facilityCategory   = category ;
};

// Display Facility info 
void Facility::DisplayFacilityInfo()
{
	std::cout << "Facility ID: " << facilityID << std::endl;
	std::cout << "Facility Name: " << facilityName << std::endl;
	std::cout << "Facility Building: " << facilityBuilding << std::endl;
	std::cout << "Facility Category: " << facilityCategory << std::endl;
	std::cout << "------------------------" << std::endl; // for readability! 
}