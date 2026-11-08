/*
* Author: Ujjwol KC
* Purpose: Manages the binary tree data structure for departments, 
*          handling file loading, case-insensitive searches, and hierarchy display.
*/
#include "../include/DepartmentTree.h"
#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

DepartmentTree::DepartmentTree() {
    root = nullptr;
    totalDepartments = 0;
}

DepartmentTree::~DepartmentTree() {
    destroyTree(root);
}

void DepartmentTree::destroyTree(DepartmentNode* node) {
    if (node == nullptr) return; 
    for (int i = 0; i < node->children.size(); i++) {
        destroyTree(node->children[i]);
    }
    delete node; 
}

string DepartmentTree::trimWhitespace(const string& str) {
    size_t p1 = str.find_first_not_of(" \t\r\n");
    if (p1 == string::npos) return ""; 
    size_t p2 = str.find_last_not_of(" \t\r\n");
    return str.substr(p1, p2 - p1 + 1);
}

string DepartmentTree::toLowerCase(const string& str) {
    string result = str;
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}

DepartmentNode* DepartmentTree::findNode(DepartmentNode* node, const string& name) {
    if (node == nullptr) return nullptr;

    if (toLowerCase(node->name) == toLowerCase(name)) {
        return node;
    }

    for (int i = 0; i < node->children.size(); i++) {
        DepartmentNode* found = findNode(node->children[i], name);
        if (found != nullptr) {
            return found; 
        }
    }

    return nullptr; 
}

bool DepartmentTree::addDepartment(const string& name, const string& parentName) {
    string cleanName = trimWhitespace(name);
    string cleanParent = trimWhitespace(parentName);

    if (cleanName.empty()) {
        cout << "Error: Department name cannot be empty.\n";
        return false;
    }

    if (root == nullptr) {
        root = new DepartmentNode(cleanName);
        totalDepartments++;
        return true;
    }

    if (findNode(root, cleanName) != nullptr) {
        cout << "Error: Department '" << cleanName << "' already exists.\n";
        return false;
    }

    DepartmentNode* parentNode = findNode(root, cleanParent);
    if (parentNode == nullptr) {
        cout << "Error: Parent department '" << cleanParent << "' not found.\n";
        return false;
    }

    DepartmentNode* newNode = new DepartmentNode(cleanName, parentNode);
    parentNode->children.push_back(newNode);
    totalDepartments++;
    
    return true;
}

void DepartmentTree::searchDepartment(const string& targetName) {
    string cleanName = trimWhitespace(targetName);
    DepartmentNode* target = findNode(root, cleanName);
    
    if (target == nullptr) {
        cout << "Department '" << cleanName << "' not found.\n";
        return;
    }

    int depth = 0;
    DepartmentNode* temp = target;
    while (temp->parent != nullptr) {
        depth++;
        temp = temp->parent; 
    }

    cout << "Department Name: " << target->name << "\n";
    if (target->parent == nullptr) {
        cout << "Parent: None (Root)\n";
    } else {
        cout << "Parent: " << target->parent->name << "\n";
    }
    cout << "Direct Children: " << target->children.size() << "\n";
    cout << "Depth: " << depth << "\n";
}

void DepartmentTree::printHierarchy(DepartmentNode* node, int depth) {
    if (node == nullptr) return;
    for (int i = 0; i < depth; i++) {
        cout << "    "; 
    }
    cout << node->name << "\n";
    for (int i = 0; i < node->children.size(); i++) {
        printHierarchy(node->children[i], depth + 1);
    }
}

void DepartmentTree::displayHierarchy() {
    if (root == nullptr) {
        cout << "No departments are loaded.\n";
        return;
    }
    printHierarchy(root, 0);
}

void DepartmentTree::printPreorder(DepartmentNode* node) {
    if (node == nullptr) return;
    cout << node->name << "\n"; 
    for (int i = 0; i < node->children.size(); i++) {
        printPreorder(node->children[i]);
    }
}

void DepartmentTree::displayPreorder() {
    if (root == nullptr) {
        cout << "Tree is empty.\n";
        return;
    }
    printPreorder(root);
}

bool DepartmentTree::loadFromFile(const string& filename) {
    ifstream file(filename);
    int accepted = 0;
    int rejected = 0;

    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << "\n";
        cout << filename << ": " << accepted << " accepted, " << rejected << " rejected\n";
        return false;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;
        
        size_t firstChar = line.find_first_not_of(" \t\r\n");
        if (firstChar == string::npos) continue; 
        if (line[firstChar] == '#') continue; 
        
        size_t delimPos1 = line.find_first_of('|');
        size_t delimPos2 = line.find_last_of('|');
        
        if (delimPos1 == string::npos || delimPos1 != delimPos2) {
            cout << "Warning: " << filename << " line " << lineNumber << " rejected (invalid delimiter format).\n";
            rejected++;
            continue;
        }

        string deptName = trimWhitespace(line.substr(0, delimPos1));
        string parentName = trimWhitespace(line.substr(delimPos1 + 1));

        if (deptName.empty() || parentName.empty()) {
            cout << "Warning: " << filename << " line " << lineNumber << " rejected (empty field).\n";
            rejected++;
            continue;
        }

        if (root == nullptr) {
            if (toLowerCase(parentName) == "none") {
                root = new DepartmentNode(deptName);
                totalDepartments++;
                accepted++;
            } else {
                cout << "Warning: " << filename << " line " << lineNumber << " rejected (first department parent must be NONE).\n";
                rejected++;
            }
            continue;
        }

        if (findNode(root, deptName) != nullptr) {
            cout << "Warning: " << filename << " line " << lineNumber << " rejected (duplicate name).\n";
            rejected++;
            continue;
        }

        DepartmentNode* pNode = findNode(root, parentName);
        if (pNode == nullptr) {
            cout << "Warning: " << filename << " line " << lineNumber << " rejected (unknown parent).\n";
            rejected++;
            continue;
        }

        DepartmentNode* newNode = new DepartmentNode(deptName, pNode);
        pNode->children.push_back(newNode);
        totalDepartments++;
        accepted++;
    }

    file.close();
    cout << filename << ": " << accepted << " accepted, " << rejected << " rejected\n";
    return true;
}