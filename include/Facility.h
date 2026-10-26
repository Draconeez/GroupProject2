#ifndef FACILITY_H
#define FACILITY_H

#include <string>
#include <iostream>
class Facility {
public:
    //Constructors
    Facility();
    Facility(std::string id, std::string name, std::string building, std::string category);
    
    //Getters and Setters
   
    std::string getFacilityID();
    std::string getFacilityName();
    std::string getFacilityBuilding();
    std::string getFacilityCatergory();

    void setFacilityID(std::string id);
    void setFacilityName(std::string name);
    void setFacilityBuilding(std::string building);
    void setFacilityCategory(std::string category);
    //Display Facility Information to the console
    void DisplayFacilityInfo();
private:
    //Types
    std::string facilityID;
    std::string facilityName;
    std::string facilityBuilding;
    std::string facilityCategory   ;
};

#endif 
