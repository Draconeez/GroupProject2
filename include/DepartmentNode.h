#ifndef DEPARTMENT_NODE_H
#define DEPARTMENT_NODE_H

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