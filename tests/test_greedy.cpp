#include <gtest/gtest.h>
#include "ann.h"

// Basic tests for the greedy search algorithm
TEST(GreedySearch, BasicTests){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    std::vector<std::unordered_set<int>> edges = {
        {1, 5},
        {0, 2},
        {0, 1, 3},
        {2, 4},
        {1, 5},
        {3, 4}
    };

    // Fixed graph initialization
    ANN<int> ann(points, edges);
    std::vector<int> start_node = {1, 2, 3};
    std::vector<int> query_node = {6, 6, 6};
    int k = 2;
    int upper_limit = 3;

    // Init the sets
    CompareVectors<int> compare(ann.node_to_point_map,query_node);
    std::set<int, CompareVectors<int>> NNS(compare);
    std::unordered_set<int> Visited;
    NNS.insert(0);

    ann.greedySearch(0, k, upper_limit, NNS, Visited, compare);
    std::set<int, CompareVectors<int>> expected(compare);
    expected.insert(1);
    expected.insert(2);
    EXPECT_EQ(NNS, expected);

    // Diffrent graph may result in different nearest neighbours
    edges = {
        {3, 4},
        {0, 3, 5},
        {1, 3},
        {0, 4, 5},
        {0, 3, 5},
        {1, 2, 3}
    };

    ANN<int> ann2(points, edges);
    NNS.clear();
    Visited.clear();
    NNS.insert(0);
    ann2.greedySearch(0, k, upper_limit, NNS, Visited, compare);

    expected.clear();
    expected.insert(0);
    expected.insert(3);
    EXPECT_EQ(NNS, expected);
}

// Test for empty query node
TEST(GreedySearch, EmptyQuery){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    std::vector<std::unordered_set<int>> edges = {
        {1, 5},
        {0, 2},
        {0, 1, 3},
        {2, 4},
        {1, 5},
        {3, 4}
    };

    ANN<int> ann(points, edges);    
    EXPECT_THROW(CompareVectors<int> compare(ann.node_to_point_map,{}),std::invalid_argument);
}

// Test Dimension mismatch
TEST(GreedySearch, DimensionMismatch){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    std::vector<std::unordered_set<int>> edges = {
        {1, 5},
        {0, 2},
        {0, 1, 3},
        {2, 4},
        {1, 5},
        {3, 4}
    };

    // Fixed graph initialization
    ANN<int> ann(points, edges);
    std::vector<int> query_node = {1,2,3,4};
    EXPECT_THROW(CompareVectors<int> compare(ann.node_to_point_map,query_node),std::invalid_argument);
}

//Test for query point being a base vector
TEST(GreedySearch, BaseVector){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    std::vector<std::unordered_set<int>> edges = {
        {1, 5},
        {0, 2},
        {0, 1, 3},
        {2, 4},
        {1, 5},
        {3, 4}
    };

    ANN<int> ann(points, edges);
    int start_node = 0;
    CompareVectors<int> compare(ann.node_to_point_map,points[0]);
    std::set<int, CompareVectors<int>> NNS(compare);
    std::unordered_set<int> Visited;
    NNS.insert(start_node);
    int k = 1;
    int upper_limit = 1;
    ann.greedySearch(start_node, k, upper_limit, NNS, Visited, compare);
    std::set<int, CompareVectors<int>> expected(compare);
    expected.insert(start_node);
    EXPECT_EQ(NNS, expected);


    NNS.clear();
    Visited.clear();
    expected.clear();
    NNS.insert(start_node);


    k = 2;
    upper_limit = 2;
    ann.greedySearch(start_node, k, upper_limit, NNS, Visited, compare);
    expected.insert(0);
    expected.insert(1);
    EXPECT_EQ(NNS, expected);
}


// Test one point graph
TEST(GreedySearch, OnePoint){
    std::vector<std::vector<int>> points = {{1, 2, 3}};
    std::vector<std::unordered_set<int>> edges = {{}};

    ANN<int> ann(points, edges);
    int start_node = 0;
    std::vector<int> query_node = {6, 6, 6};

    CompareVectors<int> compare(ann.node_to_point_map,query_node);
    std::set<int, CompareVectors<int>> NNS(compare);
    std::unordered_set<int> Visited;
    NNS.insert(start_node);
    int k = 1;
    int upper_limit = 1;

    ann.greedySearch(start_node, k, upper_limit, NNS, Visited, compare);
    std::set<int, CompareVectors<int>> expected(compare);
    expected.insert(0);
    EXPECT_EQ(NNS, expected);
} 

// Test for empty graph
TEST(GreedySearch, EmptyGraph){
    std::vector<std::vector<int>> points = {};
    std::vector<std::unordered_set<int>> edges = {};

    ANN<int> ann(points, edges);
    std::vector<int> query_node = {6, 6, 6};
    
    EXPECT_THROW(CompareVectors<int> compare(ann.node_to_point_map,query_node), std::invalid_argument); 
}

// Different k values
TEST(GreedySearch, KDifferentValues){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}};
    std::vector<std::unordered_set<int>> edges = {{1}, {0}};

    ANN<int> ann(points, edges);
    int start_node = 0;
    std::vector<int> query_node = {6, 6, 6};

    CompareVectors<int> compare(ann.node_to_point_map,query_node);
    std::set<int, CompareVectors<int>> NNS(compare);
    std::unordered_set<int> Visited;
    NNS.insert(start_node);

    // k greater than number of nodes
    int k = 3;
    int upper_limit = 3;

    ann.greedySearch(start_node, k, upper_limit, NNS, Visited, compare);
    std::set<int, CompareVectors<int>> expected(compare);
    expected.insert(0);
    expected.insert(1);
    EXPECT_EQ(NNS, expected);

    NNS.clear();
    Visited.clear();
    NNS.insert(start_node);

    // k equal to 0
    k = 0;
    ann.greedySearch(start_node, k, upper_limit, NNS, Visited, compare);
    EXPECT_TRUE(NNS.empty());


    NNS.clear();
    Visited.clear();
    NNS.insert(start_node);

    // Test upper limit less than k
    upper_limit = 1;
    k = 2;
    EXPECT_THROW(ann.greedySearch(start_node, k,upper_limit, NNS, Visited, compare), std::invalid_argument);
    EXPECT_TRUE(NNS.size() == 1);   // Only the start node
}