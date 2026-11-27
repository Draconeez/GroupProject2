/*
- Author: Ujjwol KC
- Purpose: Implements the department general tree operations, including
-          input validation, case-insensitive search, traversal, and cleanup.
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
    // Delete all children before deleting the current node so child pointers
    // are never dereferenced after their parent is freed (post-order cleanup).
    for (size_t i = 0; i < node->children.size(); i++) {
        destroyTree(node->children[i]);
    }
    delete node;
}

string DepartmentTree::trimWhitespace(const string& str) {
    // Strip spaces, tabs, and line endings (including '\r' from Windows/CRLF files)
    size_t p1 = str.find_first_not_of(" \t\r\n");
    if (p1 == string::npos) return "";
    size_t p2 = str.find_last_not_of(" \t\r\n");
    return str.substr(p1, p2 - p1 + 1);
}

string DepartmentTree::toLowerCase(const string& str) {
    string result = str;
    // Cast to unsigned char before tolower to avoid undefined behavior on negative char values
    for (char& c : result) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

DepartmentNode* DepartmentTree::findNode(DepartmentNode* node, const string& name) {
    if (node == nullptr) return nullptr;

    // Compare normalized names so searches and duplicate checks ignore capitalization
    if (toLowerCase(node->name) == toLowerCase(name)) {
        return node;
    }

    for (size_t i = 0; i < node->children.size(); i++) {
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
        cout << "======Error: Department name cannot be empty.======\n";
        return false;
    }

    // First department in an empty tree becomes the root node
    if (root == nullptr) {
        root = new DepartmentNode(cleanName);
        totalDepartments++;
        return true;
    }

    // Reject duplicates so department names stay unique across the hierarchy
    if (findNode(root, cleanName) != nullptr) {
        cout << "====== Error: Department '" << cleanName << "' already exists. ======\n";
        return false;
    }

    // Require a valid existing parent so no orphaned department nodes are created
    DepartmentNode* parentNode = findNode(root, cleanParent);
    if (parentNode == nullptr) {
        cout << "====== Error: Parent department '" << cleanParent << "' not found. ======\n";
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
        cout << "====== Department '" << cleanName << "' not found. ======\n";
        return;
    }

    // Walk upward through parent pointers to compute depth without storing extra state
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
    // Indent 4 spaces per depth level to visually show tree nesting
    for (int i = 0; i < depth; i++) {
        cout << "    ";
    }
    cout << node->name << "\n";
    for (size_t i = 0; i < node->children.size(); i++) {
        printHierarchy(node->children[i], depth + 1);
    }
}

void DepartmentTree::displayHierarchy() {
    if (root == nullptr) {
        cout << "====== No departments are loaded. ======\n";
        return;
    }
    printHierarchy(root, 0);
}

void DepartmentTree::printPreorder(DepartmentNode* node) {
    if (node == nullptr) return;
    // Visit parent before children to produce a standard preorder traversal
    cout << node->name << "\n";
    for (size_t i = 0; i < node->children.size(); i++) {
        printPreorder(node->children[i]);
    }
}

void DepartmentTree::displayPreorder() {
    if (root == nullptr) {
        cout << "====== Tree is empty. ======\n";
        return;
    }
    printPreorder(root);
}

bool DepartmentTree::loadFromFile(const string& filename) {
    ifstream file(filename);
    int accepted = 0;
    int rejected = 0;

    if (!file.is_open()) {
        cout << "====== Error: Cannot open file " << filename << " ======\n";
        cout << filename << ": " << accepted << " accepted, " << rejected << " rejected \n";
        return false;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;
        
        // Skip blank lines and '#' comment lines so data files remain readable
        size_t firstChar = line.find_first_not_of(" \t\r\n");
        if (firstChar == string::npos) continue;
        if (line[firstChar] == '#') continue;
        
        // Expect exactly one '|' separator between department name and parent name
        size_t delimPos1 = line.find_first_of('|');
        size_t delimPos2 = line.find_last_of('|');
        
        if (delimPos1 == string::npos || delimPos1 != delimPos2) {
            cout << " Warning: " << filename << " line " << lineNumber << " rejected (invalid delimiter format).\n";
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

        // The first valid entry must specify NONE as its parent to establish the root
        if (root == nullptr) {
            if (toLowerCase(parentName) == "none") {
                root = new DepartmentNode(deptName);
                totalDepartments++;
                accepted++;
            } else {
                cout << "Warning: " << filename << " line " << lineNumber << " rejected (first department parent must be NONE). \n";
                rejected++;
            }
            continue;
        }

        if (findNode(root, deptName) != nullptr) {
            cout << "Warning: " << filename << " line " << lineNumber << " rejected (duplicate name). \n";
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
    cout << filename << ": " << accepted << " accepted, " << rejected << " rejected \n";
    return true;
}