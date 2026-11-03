#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "Facility.h"
#include <string>
#include <vector>
struct HashNode {
    Facility data;
    HashNode* next;

    HashNode(const Facility& facility) : data(facility), next(nullptr) {}
};
class HashTable {
public:
    HashTable(int initCap= 17);
    ~HashTable();

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    static std::string trim(const std::string& str);
    static std::string toUpper(const std::string& str);
    static std::string toLower(const std::string& str);
    static std::string normalizeId(const std::string& rawId);
    static bool isValidIdFormat(const std::string& normalizedId);
    static std::vector<std::string> splitPipeLine(const std::string& line);
    static bool isCommentOrBlank(const std::string& line);

    // Core M1 Hash Table Operations (HASH-1 through HASH-7)
    bool insert(const Facility& facility);
    const Facility* searchById(const std::string& rawOrNormalizedId) const;


    // View all facilities 
    void displayAll() const;

    // Search facility by ID and print its details
    void searchAndPrintById(const std::string& rawInput) const;

    // Getters for hash table statistics and file order facilities
    int getCapacity() const { return capacity; }
    int getEntryCount() const { return entryCount; }
    int getCollisionCount() const { return collisionCount; }
    const std::vector<Facility>& getFacilitiesInOrder() const { return fileOrderFacilities; }
    // Loads facilities from a file into the hash table, validating and normalizing data
    void loadFromFile(const std::string& filepath, const std::vector<std::string>& acceptedBuildings);
private:
    HashNode** table;
    int capacity;
    int entryCount;
    int collisionCount;

    // Stores accepted facilities 
    std::vector<Facility> fileOrderFacilities;

    // Stores the first accepted spelling of each category 
    std::vector<std::string> canonCategories;
    
    // Computes bucket index for a given normalized ID
    int hashFunction(const std::string& normalizedId) const;

    // Returns the first accepted spelling for a category, registering it if new
    std::string resolveCategorySpelling(const std::string& rawCategory);
};

#endif 
