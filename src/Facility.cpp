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
    facilityName = name;
    facilityBuilding = building;
    facilityCategory   = category ;
};

// getters
std::string Facility::getFacilityID()
{ return facilityID; }
std::string Facility::getFacilityName()
{ return facilityName; }
std::string Facility::getFacilityBuilding()
{ return facilityBuilding; }
std::string Facility::getFacilityCategory()
{ return facilityCategory; }

// setters
void Facility::setFacilityID(std::string id)
{ facilityID = id; }
void Facility::setFacilityName(std::string name)
{ facilityName = name; }
void Facility::setFacilityBuilding(std::string building)
{ facilityBuilding = building; }
void Facility::setFacilityCategory(std::string category)
{ facilityCategory = category; }

// Display Facility info 
void Facility::DisplayFacilityInfo()
{
	std::cout << "Facility ID: " << facilityID << std::endl;
	std::cout << "Facility Name: " << facilityName << std::endl;
	std::cout << "Facility Building: " << facilityBuilding << std::endl;
	std::cout << "Facility Category: " << facilityCategory << std::endl;
	std::cout << "------------------------" << std::endl; // for readability! 
}