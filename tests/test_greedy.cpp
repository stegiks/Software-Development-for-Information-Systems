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
    ASSERT_EQ(result, expected);

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
    ASSERT_EQ(result, expected);
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
    ASSERT_EQ(result, expected);
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
    ASSERT_TRUE(result.empty());
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
    ASSERT_EQ(result, expected);

    // k equal to 0
    k = 0;
    result = ann.greedySearch(start_node, query_node, k, upper_limit);
    ASSERT_TRUE(result.empty());
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
    ASSERT_EQ(result, expected);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}