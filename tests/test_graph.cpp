#include <gtest/gtest.h>
#include "graph.h"

// Test if the graph is correctly initialized with random edges
TEST(GraphTest, RandomInit){
    Graph graph(10);

    EXPECT_EQ(graph.countNeighbours(0), (int)graph.getNeighbours(0).size());
    EXPECT_EQ(graph.countNeighbours(5), (int)graph.getNeighbours(5).size());
    EXPECT_EQ(graph.countNeighbours(9), (int)graph.getNeighbours(9).size());
}

// Test for graph similarity
TEST(GraphTest, CheckSimilarity){
    std::vector<std::unordered_set<int>> edges1 = {
        {1, 2},
        {0, 3},
        {0, 3},
        {1, 2}
    };

    std::vector<std::unordered_set<int>> edges2 = {
        {1, 2},
        {0, 3},
        {0, 3},
        {1, 2}
    };

    std::vector<std::unordered_set<int>> edges3 = {
        {1, 2},
        {0, 3},
        {0, 4},
        {1, 2}
    };

    Graph graph1(edges1);

    ASSERT_TRUE(graph1.checkSimilarity(edges2));
    ASSERT_FALSE(graph1.checkSimilarity(edges3));
}

// If I give a specific set of edges, then the graph should be initialized with those edges
TEST(GraphTest, SpecificEdges){
    std::vector<std::unordered_set<int>> edges = {
        {1, 3, 4},
        {0, 2, 3},
        {1, 4},
        {0, 1, 4},
        {0, 2, 3}
    };

    Graph graph(edges);

    // Neighbours of the first node should be 3
    EXPECT_EQ(graph.countNeighbours(0), 3);
    
    // Neighbours of the first node have specific values
    const std::unordered_set<int>& neighbours_of_first = graph.getNeighbours(0);
    std::unordered_set<int> expected = {1, 3, 4};
    EXPECT_EQ(neighbours_of_first, expected);

    // Check if the graph is correctly initialized
    EXPECT_TRUE(graph.checkSimilarity(edges));
}

// Test addEdge and removeEdge functions
TEST(GraphTest, AddRemoveEdge){
    std::vector<std::unordered_set<int>> edges = {
        {}, {}, {}, {}, {}
    };
    Graph graph(edges);

    // Add edges
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);

    EXPECT_EQ(graph.countNeighbours(0), 3);
    EXPECT_EQ(graph.getNeighbours(0), std::unordered_set<int>({1, 2, 3}));

    // Remove edges
    graph.removeEdge(0, 1);
    EXPECT_EQ(graph.countNeighbours(0), 2);
    EXPECT_EQ(graph.getNeighbours(0), std::unordered_set<int>({2, 3}));

    // Remove all edges
    graph.removeNeighbours(0);
    EXPECT_EQ(graph.countNeighbours(0), 0);
    EXPECT_EQ(graph.getNeighbours(0), std::unordered_set<int>());
}

// Test unexpected actions that should not affect the graph
TEST(GraphTest, UnexpectedActions){
    std::vector<std::unordered_set<int>> edges = {
        {1, 3, 4},
        {0, 2, 3},
        {},
        {0, 1, 4},
        {0, 2, 3}
    };

    Graph graph(edges);

    // Try to remove a non-existent edge
    graph.removeEdge(0, 2);
    std::unordered_set<int> expected = {1, 3, 4};
    EXPECT_EQ(graph.getNeighbours(0), expected);

    // Try adding the same edge twice
    graph.addEdge(0, 2);
    graph.addEdge(0, 2);
    expected.insert(2);
    EXPECT_EQ(graph.getNeighbours(0), expected);

    // Remove neighbours when there are none
    graph.removeNeighbours(2);
    EXPECT_EQ(graph.countNeighbours(2), 0);

    // Access an out of bounds node
    EXPECT_THROW(graph.getNeighbours(10), std::out_of_range);
}

// Test for enforcing a regular graph degree
TEST(GraphTest, EnforceRegularDegree){
    std::vector<std::unordered_set<int>> edges = {
        {1, 2},       
        {0},          
        {0, 3, 4},    
        {2, 4},       
        {2, 3}        
    };

    Graph graph(edges);
    int R = 2;
    graph.enforceRegular(R);

    // Check that each node has exactly R neighbors
    for(std::size_t i = 0; i < edges.size(); i++){
        EXPECT_EQ(graph.countNeighbours(i), R) << "Node " << i << " does not have " << R << " neighbors.";
    }
}