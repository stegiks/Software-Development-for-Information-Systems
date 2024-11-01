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
    CompareVectors<int> compare(query_node);
    std::set<std::vector<int>, CompareVectors<int>> NNS(compare);
    std::unordered_set<std::vector<int>, VectorHash<int>> Visited;
    NNS.insert(start_node);

    ann.greedySearch(start_node, query_node, k, upper_limit, NNS, Visited, compare);
    std::set<std::vector<int>, CompareVectors<int>> expected(compare);
    expected.insert({4, 5, 6});
    expected.insert({7, 8, 9});
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
    NNS.insert(start_node);
    ann2.greedySearch(start_node, query_node, k, upper_limit, NNS, Visited, compare);

    expected.clear();
    expected.insert({1, 2, 3});
    expected.insert({10, 11, 12});
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
    std::vector<int> start_node = {1, 2, 3};
    int k = 2;
    int upper_limit = 3;
    CompareVectors<int> compare({});
    std::set<std::vector<int>, CompareVectors<int>> NNS(compare);
    std::unordered_set<std::vector<int>, VectorHash<int>> Visited;
    NNS.insert(start_node);

    ann.greedySearch(start_node, {}, k, upper_limit, NNS, Visited, compare);

    
    EXPECT_TRUE(NNS.empty());
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
    std::vector<int> start_node = {1, 2, 3};
    std::vector<int> query_node = {1,2,3,4};
    CompareVectors<int> compare(query_node);
    std::set<std::vector<int>, CompareVectors<int>> NNS(compare);
    std::unordered_set<std::vector<int>, VectorHash<int>> Visited;
    NNS.insert(start_node);
    int k = 2;
    int upper_limit = 3;

    ann.greedySearch(start_node, query_node, k, upper_limit, NNS, Visited, compare);
    EXPECT_TRUE(NNS.empty());
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
    std::vector<int> start_node = {1, 2, 3};
    CompareVectors<int> compare(start_node);
    std::set<std::vector<int>, CompareVectors<int>> NNS(compare);
    std::unordered_set<std::vector<int>, VectorHash<int>> Visited;
    NNS.insert(start_node);
    int k = 1;
    int upper_limit = 1;
    ann.greedySearch(start_node, start_node, k, upper_limit, NNS, Visited, compare);
    std::set<std::vector<int>, CompareVectors<int>> expected(compare);
    expected.insert(start_node);
    EXPECT_EQ(NNS, expected);


    NNS.clear();
    Visited.clear();
    expected.clear();
    NNS.insert(start_node);


    k = 2;
    upper_limit = 2;
    ann.greedySearch(start_node, start_node, k, upper_limit, NNS, Visited, compare);
    expected.insert({1, 2, 3});
    expected.insert({4, 5, 6});
    EXPECT_EQ(NNS, expected);
}


// Test one point graph
TEST(GreedySearch, OnePoint){
    std::vector<std::vector<int>> points = {{1, 2, 3}};
    std::vector<std::unordered_set<int>> edges = {{}};

    ANN<int> ann(points, edges);
    std::vector<int> start_node = {1, 2, 3};
    std::vector<int> query_node = {6, 6, 6};

    CompareVectors<int> compare(query_node);
    std::set<std::vector<int>, CompareVectors<int>> NNS(compare);
    std::unordered_set<std::vector<int>, VectorHash<int>> Visited;
    NNS.insert(start_node);
    int k = 1;
    int upper_limit = 1;

    ann.greedySearch(start_node, query_node, k, upper_limit, NNS, Visited, compare);
    std::set<std::vector<int>, CompareVectors<int>> expected(compare);
    expected.insert({1, 2, 3});
    EXPECT_EQ(NNS, expected);
} 

// Test for empty graph
TEST(GreedySearch, EmptyGraph){
    std::vector<std::vector<int>> points = {};
    std::vector<std::unordered_set<int>> edges = {};

    ANN<int> ann(points, edges);
    std::vector<int> start_node = {1, 2, 3};
    std::vector<int> query_node = {6, 6, 6};
    
    CompareVectors<int> compare(query_node);
    std::set<std::vector<int>, CompareVectors<int>> NNS(compare);
    std::unordered_set<std::vector<int>, VectorHash<int>> Visited;
    NNS.insert(start_node);
    int k = 2;
    int upper_limit = 3;

    ann.greedySearch(start_node, query_node, k, upper_limit, NNS, Visited, compare);
    EXPECT_TRUE(NNS.empty());
}

// Different k values
TEST(GreedySearch, KDifferentValues){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}};
    std::vector<std::unordered_set<int>> edges = {{1}, {0}};

    ANN<int> ann(points, edges);
    std::vector<int> start_node = {1, 2, 3};
    std::vector<int> query_node = {6, 6, 6};

    CompareVectors<int> compare(query_node);
    std::set<std::vector<int>, CompareVectors<int>> NNS(compare);
    std::unordered_set<std::vector<int>, VectorHash<int>> Visited;
    NNS.insert(start_node);

    // k greater than number of nodes
    int k = 3;
    int upper_limit = 3;

    ann.greedySearch(start_node, query_node, k, upper_limit, NNS, Visited, compare);
    std::set<std::vector<int>, CompareVectors<int>> expected(compare);
    expected.insert({1, 2, 3});
    expected.insert({4, 5, 6});
    EXPECT_EQ(NNS, expected);


    NNS.clear();
    Visited.clear();
    NNS.insert(start_node);

    // k equal to 0
    k = 0;
    ann.greedySearch(start_node, query_node, k, upper_limit, NNS, Visited, compare);
    EXPECT_TRUE(NNS.empty());


    NNS.clear();
    Visited.clear();
    NNS.insert(start_node);

    // Test upper limit less than k
    upper_limit = 1;
    k = 2;
    ann.greedySearch(start_node, query_node, k,upper_limit, NNS, Visited, compare);
    EXPECT_TRUE(NNS.empty());
}

// Test for unsigned char datatype
TEST(GreedySearch, UnsignedCharDatatype){
    std::vector<std::vector<unsigned char>> points = {{'a', 'd', 'k'}, {'b', 'l', 'm'}, {'c', 'n', 'o'}, {'t', 't', 't'}, {'x', 'y', 'z'}, {'a', 'b', 'c'}};
    std::vector<std::unordered_set<int>> edges = {
        {1, 4},
        {0, 3, 5},
        {1, 3},
        {2, 4, 5},
        {0, 3, 5},
        {1, 4}
    };

    ANN<unsigned char> ann(points, edges);
    std::vector<unsigned char> start_node = {'a', 'd', 'k'};
    std::vector<unsigned char> query_node = {'m', 'm', 'm'};
    
    CompareVectors<unsigned char> compare(query_node);
    std::set<std::vector<unsigned char>, CompareVectors<unsigned char>> NNS(compare);
    std::unordered_set<std::vector<unsigned char>, VectorHash<unsigned char>> Visited;
    NNS.insert(start_node);

    
    int k = 2;
    int upper_limit = 3;


    ann.greedySearch(start_node, query_node, k, upper_limit, NNS, Visited, compare);
    std::set<std::vector<unsigned char>, CompareVectors<unsigned char>> expected(compare);
    expected.insert({'b', 'l', 'm'});
    expected.insert({'c', 'n', 'o'});
    EXPECT_EQ(NNS, expected);
}

// Test for float datatype
TEST(GreedySearch, FloatDatatype){
    std::vector<std::vector<float>> points = {{1.1, 2.2, 3.3}, {4.4, 5.5, 6.6}, {7.7, 8.8, 9.9}, {10.1, 11.1, 12.1}, {13.1, 14.1, 15.1}, {16.1, 17.1, 18.1}};
    std::vector<std::unordered_set<int>> edges = {
        {1, 5},
        {0, 2},
        {0, 1, 3},
        {2, 4},
        {1, 5},
        {3, 4}
    };

    ANN<float> ann(points, edges);
    std::vector<float> start_node = {1.1, 2.2, 3.3};
    std::vector<float> query_node = {6.6, 6.6, 6.6};

    CompareVectors<float> compare(query_node);
    std::set<std::vector<float>, CompareVectors<float>> NNS(compare);
    std::unordered_set<std::vector<float>, VectorHash<float>> Visited;
    NNS.insert(start_node);

    int k = 2;
    int upper_limit = 3;

    ann.greedySearch(start_node, query_node, k, upper_limit, NNS, Visited, compare);
    std::set<std::vector<float>, CompareVectors<float>> expected(compare);
    expected.insert({4.4, 5.5, 6.6});
    expected.insert({7.7, 8.8, 9.9});
    EXPECT_EQ(NNS, expected);
}
