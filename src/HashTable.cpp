
#include "../include/HashTable.h"
#include <cctype>
#include <fstream>
#include <iostream>

// Initializes an empty separate-chaining hash table with the given capacity 
HashTable::HashTable(int initialCapacity) {
    if (initialCapacity > 0) {
        capacity = initialCapacity;
    } 
    else 
    {
        capacity = 17;
    }
    entryCount = 0;
    collisionCount = 0;
    table = new HashNode*[capacity];
    for (int i = 0; i < capacity; i++) {
        table[i] = nullptr;
    }
}

// Frees all dynamically allocated HashNodes and the bucket array 
HashTable::~HashTable() {
    for (int i = 0; i < capacity; ++i) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }
    delete[] table;
}

//Trims leading and trailing spaces, tabs, carriage returns, and newlines 
std::string HashTable::trim(const std::string& str) {
    const std::string whitespace = " \t\r\n";
    size_t first = str.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(whitespace);
    return str.substr(first, last - first + 1);
}

//Converts all ASCII letters in a string to uppercase
std::string HashTable::toUpper(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(result[i])));
    }
    return result;
}

//Converts all ASCII letters in a string to lowercase
std::string HashTable::toLower(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}

//Trims outer whitespace and converts a facility ID to uppercase 
std::string HashTable::normalizeId(const std::string& rawId) {
    return toUpper(trim(rawId));
}

// Checks that a normalized ID is 'F' followed by exactly 3 ASCII digits 
bool HashTable::isValidIdFormat(const std::string& normalizedId) {
    if (normalizedId.length() != 4) {
        return false;
    }
    if (normalizedId[0] != 'F') {
        return false;
    }

    // Make sure the last 3 characters are digits
    for (int i = 1; i < 4; i++) {
        if (!std::isdigit(normalizedId[i])) {
            return false;
        }
    }
    return true;
}

// Splits a line into separate fields wherever there is a '|' character 
std::vector<std::string> HashTable::splitPipeLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string currentField = "";

    for (int i = 0; i < (int)line.length(); i++) {
        if (line[i] == '|') {
            fields.push_back(trim(currentField));
            currentField = "";
        } else {
            currentField += line[i];
        }
    }

    // Add the last field after the final '|'
    fields.push_back(trim(currentField));
    return fields;
}

// Returns true if a line is blank or starts with '#'
bool HashTable::isCommentOrBlank(const std::string& line) {
    std::string cleaned = trim(line);
    if (cleaned.empty()) {
        return true;
    }
    if (cleaned[0] == '#') {
        return true;
    }
    return false;
}

// Custom hash function that turns an ID string into a bucket index 
int HashTable::hashFunction(const std::string& normalizedId) const {
    int hashValue = 0;

    for (int i = 0; i < (int)normalizedId.length(); i++) {
        hashValue = (hashValue * 31 + normalizedId[i]) % capacity;
    }

    // Ensure the index is positive
    if (hashValue < 0) {
        hashValue += capacity;
    }
    return hashValue;
}

//Keeps the spelling of the first accepted record for each category 
std::string HashTable::resolveCategorySpelling(const std::string& rawCategory) {
    std::string trimmedCat = trim(rawCategory);
    std::string lowerCat = toLower(trimmedCat);

    for (int i = 0; i < (int)canonCategories.size(); i++) {
        if (toLower(canonCategories[i]) == lowerCat) {
            return canonCategories[i];
        }
    }

    canonCategories.push_back(trimmedCat);
    return trimmedCat;
}

//Inserts a Facility into the hash table; rejects duplicates
bool HashTable::insert(const Facility& facility) {
    std::string normId = normalizeId(facility.getFacilityID());
    if (!isValidIdFormat(normId)) {
        return false;
    }

    int index = hashFunction(normId);
    HashNode* current = table[index];
    HashNode* tail = nullptr;

    //Walk the linked list at this bucket to check for a duplicate ID
    while (current != nullptr) {
        if (current->data.getFacilityID() == normId) {
            return false; 
        
        }
        tail = current;
        current = current->next;
    }

    // If the bucket already had at least one node, count a collision 
    if (table[index] != nullptr) {
        collisionCount++;
    }

    std::string canonicalCat = resolveCategorySpelling(facility.getFacilityCategory());
    Facility acceptedRecord(normId,
                            trim(facility.getFacilityName()),
                            trim(facility.getFacilityBuilding()),
                            canonicalCat);

    // Create the new node and attach it to the bucket's linked list
    HashNode* newNode = new HashNode(acceptedRecord);
    if (tail == nullptr) {
        table[index] = newNode;
    } else {
        tail->next = newNode;
    }

    fileOrderFacilities.push_back(acceptedRecord);
    entryCount++;
    return true;
}

// Looks up a facility by ID in the hash table
const Facility* HashTable::searchById(const std::string& rawOrNormalizedId) const {
    std::string normId = normalizeId(rawOrNormalizedId);
    if (!isValidIdFormat(normId)) {
        return nullptr;
    }

    int index = hashFunction(normId);
    HashNode* current = table[index];

    while (current != nullptr) {
        if (current->data.getFacilityID() == normId) {
            return &(current->data);
        }
        current = current->next;
    }

    return nullptr;
}

//Print all facilities in file order
void HashTable::displayAll() const {
    if (fileOrderFacilities.empty()) {
        std::cout << "No facilities are loaded.\n";
        std::cout << "Total facilities: 0\n";
        return;
    }

    for (int i = 0; i < (int)fileOrderFacilities.size(); i++) {
        fileOrderFacilities[i].DisplayFacilityInfo();
    }
    std::cout << "Total facilities: " << fileOrderFacilities.size() << "\n";
}

// Check ID format, search hash table, and print results
void HashTable::searchAndPrintById(const std::string& rawInput) const {
    std::string normId = normalizeId(rawInput);

    //OC1: Invalid ID format
    if (!isValidIdFormat(normId)) {
        std::cout << "Invalid format: an ID is F followed by exactly three digits\n";
        return;
    }

    const Facility* found = searchById(normId);

    //OC2: Valid format, but not found
    if (found == nullptr) {
        std::cout << "Not found: no facility with ID " << normId << "\n";
        return;
    }

    //OC3: Found
    std::cout << "Found: " << found->getFacilityName() << " | "
              << found->getFacilityBuilding() << " | "
              << found->getFacilityCategory() << "\n";
}