#include <gtest/gtest.h>
#include "ann.h"

// Basic tests for the greedy search algorithm
TEST(GreedySearch, BasicTests){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    std::vector<std::vector<int>> edges = {
        {0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0},
        {1, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 0}
    };

    // Fixed graph initialization
    ANN<int> ann(points, edges);
    std::vector<int> start_node = {1, 2, 3};
    std::vector<int> query_node = {6, 6, 6};
    int k = 2;
    int upper_limit = 3;

    std::set<std::vector<int>> result = ann.greedySearch(start_node, query_node, k, upper_limit);
    std::set<std::vector<int>> expected = {{4, 5, 6}, {7, 8, 9}};
    EXPECT_EQ(result, expected);

    // Diffrent graph may result in different nearest neighbours
    edges = {
        {0, 0, 0, 1, 1, 0},
        {1, 0, 0, 1, 0, 1},
        {0, 1, 0, 1, 0, 0},
        {1, 0, 0, 0, 1, 1},
        {1, 0, 0, 1, 0, 1},
        {0, 1, 1, 1, 0, 0}
    };

    ANN<int> ann2(points, edges);
    result = ann2.greedySearch(start_node, query_node, k, upper_limit);
    expected = {{1, 2, 3}, {10, 11, 12}};
    EXPECT_EQ(result, expected);
}

// Test for empty query node
TEST(GreedySearch, EmptyQuery){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    std::vector<std::vector<int>> edges = {
        {0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0},
        {1, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 0}
    };

    // Fixed graph initialization
    ANN<int> ann(points, edges);
    std::set<std::vector<int>> query = {};
    std::set<std::vector<int>> result = {};

    int k = 2;
    int upper_limit = 3;
    result = ann.greedySearch({1, 2, 3}, {}, k, upper_limit);
    EXPECT_TRUE(result.empty());
}

// Test Dimension mismatch
TEST(GreedySearch, DimensionMismatch){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    std::vector<std::vector<int>> edges = {
        {0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0},
        {1, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 0}
    };

    // Fixed graph initialization
    ANN<int> ann(points, edges);
    std::set<std::vector<int>> query = {};
    std::set<std::vector<int>> result = {};

    int k = 2;
    int upper_limit = 3;

    result = ann.greedySearch({1, 2, 3}, {1, 2, 3, 4}, k, upper_limit);
    EXPECT_TRUE(result.empty());
}

//Test for query point being a base vector
TEST(GreedySearch, BaseVector){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    std::vector<std::vector<int>> edges = {
        {0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0},
        {1, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 0}
    };

    ANN<int> ann(points, edges);
    std::vector<int> start_node = {1, 2, 3};

    int k = 1;
    int upper_limit = 1;
    std::set<std::vector<int>> result = ann.greedySearch(start_node, start_node, k, upper_limit);
    std::set<std::vector<int>> expected = {{1, 2, 3}};
    EXPECT_EQ(result, expected);


    k = 2;
    upper_limit = 2;
    std::set<std::vector<int>> result2 = ann.greedySearch(start_node, start_node, k, upper_limit);
    expected = {{1, 2, 3}, {4, 5, 6}};
    EXPECT_EQ(result2, expected);
}


// Test one point graph
TEST(GreedySearch, OnePoint){
    std::vector<std::vector<int>> points = {{1, 2, 3}};
    std::vector<std::vector<int>> edges = {{0}};

    ANN<int> ann(points, edges);
    std::vector<int> start_node = {1, 2, 3};
    std::vector<int> query_node = {6, 6, 6};
    int k = 1;
    int upper_limit = 1;

    std::set<std::vector<int>> result = ann.greedySearch(start_node, query_node, k, upper_limit);
    std::set<std::vector<int>> expected = {{1, 2, 3}};
    EXPECT_EQ(result, expected);
} 

// Test for empty graph
TEST(GreedySearch, EmptyGraph){
    std::vector<std::vector<int>> points = {};
    std::vector<std::vector<int>> edges = {};

    ANN<int> ann(points, edges);
    std::vector<int> start_node = {1, 2, 3};
    std::vector<int> query_node = {6, 6, 6};
    int k = 2;
    int upper_limit = 3;

    std::set<std::vector<int>> result = ann.greedySearch(start_node, query_node, k, upper_limit);
    EXPECT_TRUE(result.empty());
}

// Different k values
TEST(GreedySearch, KDifferentValues){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}};
    std::vector<std::vector<int>> edges = {{0, 1}, {1, 0}};

    ANN<int> ann(points, edges);
    std::vector<int> start_node = {1, 2, 3};
    std::vector<int> query_node = {6, 6, 6};

    // k greater than number of nodes
    int k = 3;
    int upper_limit = 3;

    std::set<std::vector<int>> result = ann.greedySearch(start_node, query_node, k, upper_limit);
    std::set<std::vector<int>> expected = {{1, 2, 3}, {4, 5, 6}};
    EXPECT_EQ(result, expected);

    // k equal to 0
    k = 0;
    result = ann.greedySearch(start_node, query_node, k, upper_limit);
    EXPECT_TRUE(result.empty());


    // Test upper limit less than k
    upper_limit = 1;
    k = 2;
    result = ann.greedySearch(start_node, query_node, k,upper_limit);
    expected = {};
    EXPECT_EQ(result, expected);
}

// Test for unsigned char datatype
TEST(GreedySearch, UnsignedCharDatatype){
    std::vector<std::vector<unsigned char>> points = {{'a', 'd', 'k'}, {'b', 'l', 'm'}, {'c', 'n', 'o'}, {'t', 't', 't'}, {'x', 'y', 'z'}, {'a', 'b', 'c'}};
    std::vector<std::vector<int>> edges = {
        {0, 1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0, 1},
        {0, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 1},
        {1, 0, 0, 1, 0, 1},
        {0, 1, 0, 0, 1, 0}
    };

    ANN<unsigned char> ann(points, edges);
    std::vector<unsigned char> start_node = {'a', 'd', 'k'};
    std::vector<unsigned char> query_node = {'m', 'm', 'm'};
    int k = 2;
    int upper_limit = 3;

    std::set<std::vector<unsigned char>> result = ann.greedySearch(start_node, query_node, k, upper_limit);
    std::set<std::vector<unsigned char>> expected = {{'b', 'l', 'm'}, {'c', 'n', 'o'}};
    EXPECT_EQ(result, expected);
}

// Test for float datatype
TEST(GreedySearch, FloatDatatype){
    std::vector<std::vector<float>> points = {{1.1, 2.2, 3.3}, {4.4, 5.5, 6.6}, {7.7, 8.8, 9.9}, {10.1, 11.1, 12.1}, {13.1, 14.1, 15.1}, {16.1, 17.1, 18.1}};
    std::vector<std::vector<int>> edges = {
        {0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0},
        {1, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 0}
    };

    ANN<float> ann(points, edges);
    std::vector<float> start_node = {1.1, 2.2, 3.3};
    std::vector<float> query_node = {6.6, 6.6, 6.6};
    int k = 2;
    int upper_limit = 3;

    std::set<std::vector<float>> result = ann.greedySearch(start_node, query_node, k, upper_limit);
    std::set<std::vector<float>> expected = {{4.4, 5.5, 6.6}, {7.7, 8.8, 9.9}};
    EXPECT_EQ(result, expected);
}
