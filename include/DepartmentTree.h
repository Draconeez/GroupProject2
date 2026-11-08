/*
* Author: Ujjwol KC
* Purpose: Manages the binary tree data structure for departments, 
*          handling file loading, case-insensitive searches, and hierarchy display.
*/
#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include "DepartmentNode.h"
#include <string>

class DepartmentTree {
private:
    DepartmentNode* root;
    int totalDepartments;

    // Helper functions for memory and recursive logic
    void destroyTree(DepartmentNode* node);
    DepartmentNode* findNode(DepartmentNode* node, const std::string& name);
    void printHierarchy(DepartmentNode* node, int depth);
    void printPreorder(DepartmentNode* node);
    
    // String processing helpers
    std::string toLowerCase(const std::string& str);
    std::string trimWhitespace(const std::string& str);

public:
    DepartmentTree();
    ~DepartmentTree();

    // Prevent accidental shallow copying (Rule of Three)
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    // Helper for main.cpp menu integration
    bool isEmpty() const { return root == nullptr; }

    // Core Milestone 1 Requirements
    bool loadFromFile(const std::string& filename);
    bool addDepartment(const std::string& name, const std::string& parentName);
    void searchDepartment(const std::string& name);
    void displayHierarchy();
    void displayPreorder();
};

#endif