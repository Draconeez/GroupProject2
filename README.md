# GroupProject2
The Second Group Project

To run, g++ -std=c++17 -Iinclude src/*.cpp -o groupProj2

./groupProj2

to use with test data

./groupProj2 testdata

# Repo Link
https://github.com/Draconeez/GroupProject2

# Project Members
- Aryn
- Will
- KC

# Smart Campus Navigation and Facility Analytics System

This program loads campus buildings, walkways, facilities, and academic
departments from pipe-delimited text files and provides an interactive menu to:
- View all facilities in file order and look up facilities by ID using a custom
  separate-chaining hash table.
- View the campus building graph, vertex degrees, and BFS reachability.
- Manage and traverse the academic department general tree.

GroupProject2/
|-- include/             Header files (Facility.h, HashTable.h, CampusGraph.h, DepartmentTree.h)
|-- src/                 Source files (main.cpp, Facility.cpp, HashTable.cpp, etc.)
|-- data/                Instructor-provided starter input files (unchanged)
|-- testdata/            Custom test files for collisions and edge cases
|-- README.txt           Build/run instructions and layout
|-- CONTRIBUTIONS.txt    Team member module ownership and shared work
|-- design_document.txt  Architecture, hash function, collision strategy, and graph design
|-- test_results.txt     Milestone 1 test cases and results
|-- .gitignore           Excludes compiled binaries and object files


