#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <ctime>

// Class to represent a graph
class Graph{
private:
    // Adjacency matrix to store graph edges
    std::vector<std::vector<int> > adj_matrix;

public:
    Graph(std::size_t n);

    // Function to add and remove edges
    void addEdge(int a, int b);
    void removeEdge(int a, int b);
    
    //Function to get neighbours of a node
    std::vector<int> getNeighbours(int node);

    void removeNeighbours(int node);

    int countNeighbours(int node);
};

#endif // graph.h