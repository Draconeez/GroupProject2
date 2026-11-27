/*
- Author: Ujjwol KC
- Purpose: Manages the general tree data structure for academic departments,
-          handling file loading, case-insensitive searches, and hierarchy display.
*/
#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include "DepartmentNode.h"
#include <string>

class DepartmentTree {
private:
    DepartmentNode* root;
    int totalDepartments;

    // Recursive helpers for memory cleanup, lookup, and tree output
    void destroyTree(DepartmentNode* node);
    DepartmentNode* findNode(DepartmentNode* node, const std::string& name);
    void printHierarchy(DepartmentNode* node, int depth);
    void printPreorder(DepartmentNode* node);
    
    // Normalize user/file strings so minor formatting differences do not fail lookups
    std::string toLowerCase(const std::string& str);
    std::string trimWhitespace(const std::string& str);

public:
    DepartmentTree();
    ~DepartmentTree();

    // Delete copy constructor and copy assignment to prevent accidental shallow copies
    // of raw heap pointers (Rule of Three / Rule of Five safety).
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    // Allows main.cpp to check tree state before running menu operations
    bool isEmpty() const { return root == nullptr; }

    // Core DepartmentTree operations
    bool loadFromFile(const std::string& filename);
    bool addDepartment(const std::string& name, const std::string& parentName);
    void searchDepartment(const std::string& name);
    void displayHierarchy();
    void displayPreorder();
};

#endif