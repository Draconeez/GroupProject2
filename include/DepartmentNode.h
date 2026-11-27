#ifndef DEPARTMENT_NODE_H
#define DEPARTMENT_NODE_H
/*
- Author: Ujjwol KC
- Purpose: Defines the node structure for the department general tree,
-          storing department names and parent/child relationships.
*/
#include <string>
#include <vector>

class DepartmentNode {
public:
    std::string name;
    DepartmentNode* parent;
    // Use a vector of child pointers instead of left/right pointers so a
    // college or department can have any number of sub-departments.
    std::vector<DepartmentNode*> children;

    DepartmentNode(std::string deptName, DepartmentNode* parentNode = nullptr) {
        name = deptName;
        parent = parentNode;
    }
};

#endif