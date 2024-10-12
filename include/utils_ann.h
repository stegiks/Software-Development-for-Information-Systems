#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include "defs.h"

// Function to find the extension of a file
std::string findExtension(const std::string& file_path);

// Function to validate the extension of the files
bool validateExtension(const std::string& extension_base, const std::string& extension_query, const std::string& extension_gt);

// Class to represent a graph
class Graph{
private:
    // Adjacency matrix to store graph edges
    std::vector<std::vector<int> > adj_matrix;

public:
    Graph(int);

    // Function to add and remove edges
    void addEdge(int a, int b);
    void removeEdge(int a, int b);
    
    //Function to get neighbours of a node
    std::vector<int> getNeighbours(int a);
};
#endif // utils.h