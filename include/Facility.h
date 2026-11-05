//File: Facility.h
//Purpose: Declare the Facility class, which represents a facility with an ID, name, building, and category.
//Author: Aryn
#ifndef FACILITY_H
#define FACILITY_H

#include <string>
class Facility {
public:
    //Constructors
    Facility();
    Facility(const std::string& id, const std::string& name, const std::string& building, const std::string& category);
    
    //Getters and Setters
   
    std::string getFacilityID() const;
    std::string getFacilityName() const;
    std::string getFacilityBuilding() const;
    std::string getFacilityCategory() const;

    void setFacilityID(const std::string& id);
    void setFacilityName(const std::string& name);
    void setFacilityBuilding(const std::string& building);
    void setFacilityCategory(const std::string& category);

    //Display Facility Information to the console
    void DisplayFacilityInfo() const;
private:
    //Types
    std::string facilityID;
    std::string facilityName;
    std::string facilityBuilding;
    std::string facilityCategory;
};

#endif 
