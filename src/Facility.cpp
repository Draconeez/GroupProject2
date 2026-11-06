//File: Facility.cpp
//Purpose: Implements the Facility class, which represents a facility with an ID, name, building, and category.
//Author: Aryn
#include "../include/Facility.h"
#include <iostream>
//Constructors
Facility::Facility()
{
    facilityID = "";
    facilityName  = "";
    facilityBuilding = "";
    facilityCategory   = "";
};
Facility::Facility(const std::string& id, const std::string& name, const std::string& building, const std::string& category)
{
    facilityID  = id;
    facilityName = name;
    facilityBuilding = building;
    facilityCategory   = category;
};

// getters
std::string Facility::getFacilityID() const
{ return facilityID; }
std::string Facility::getFacilityName() const
{ return facilityName; }
std::string Facility::getFacilityBuilding() const
{ return facilityBuilding; }
std::string Facility::getFacilityCategory() const
{ return facilityCategory; }

// setters
void Facility::setFacilityID(const std::string& id)
{ facilityID = id; }
void Facility::setFacilityName(const std::string& name)
{ facilityName = name; }
void Facility::setFacilityBuilding(const std::string& building)
{ facilityBuilding = building; }
void Facility::setFacilityCategory(const std::string& category)
{ facilityCategory = category; }

// Display Facility info 
void Facility::DisplayFacilityInfo() const
{
	std::cout << "Facility ID: " << facilityID << std::endl;
	std::cout << "Facility Name: " << facilityName << std::endl;
	std::cout << "Facility Building: " << facilityBuilding << std::endl;
	std::cout << "Facility Category: " << facilityCategory << std::endl;
	std::cout << "------------------------" << std::endl; // for readability! 
}