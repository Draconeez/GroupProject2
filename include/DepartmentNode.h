#ifndef DEPARTMENT_NODE_H
#define DEPARTMENT_NODE_H
/*
* Author: Ujjwol KC
* Purpose: Defines the node structure for the department binary tree, 
*          storing department names and parent/child relationships.
*/
#include <string>
#include <vector>

class DepartmentNode {
public:
    std::string name;
    DepartmentNode* parent;
    std::vector<DepartmentNode*> children;

    DepartmentNode(std::string deptName, DepartmentNode* parentNode = nullptr) {
        name = deptName;
        parent = parentNode;
    }
};

#endif