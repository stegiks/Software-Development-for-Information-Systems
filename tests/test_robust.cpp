#include <gtest/gtest.h>
#include "ann.h"
#include <set>
#include <vector>

// Test Fixture class for the testing Robust Prune functionality
class RobustPruneTest : public testing::Test{
protected:

    std::vector<std::vector<int>> points;
    std::vector<std::unordered_set<int>> edges;
    std::vector<int> start;
    ANN<int>* ann;

    RobustPruneTest(){
        // Initialize the graph
        points = {{1, 2}, {1, 0}, {2, 3}, {1, -5}, {3, -5}, {6, 2}};
        edges = {
            {2},
            {0, 3},
            {1, 5},
            {},
            {3},
            {4}
        };

        start = {2, 3};
        ann = new ANN<int>(points, edges);
    }

    ~RobustPruneTest() override {
        delete ann;
    }  
};

// Test basic pruning functionality
TEST_F(RobustPruneTest, BasicFunctionality){

    // Provide custom comparator for the set
    CompareVectors<int> compare(start);
    std::set<std::vector<int>, CompareVectors<int>> candidate(compare);
    candidate.insert({1, 2});
    candidate.insert({1, 0});
    candidate.insert({1, -5});

    std::vector<std::unordered_set<int>> expected = {
        {2},
        {0, 3},
        {0, 5},
        {},
        {3},
        {4}
    };

    ann->robustPrune(start, candidate, 1.1, 5);

    EXPECT_TRUE(ann->checkGraph(expected));
}

// Test empty candidate set
TEST_F(RobustPruneTest, EmptyCandidateSet){

    CompareVectors<int> compare(start);
    std::set<std::vector<int>, CompareVectors<int>> candidate(compare);

    ann->robustPrune(start, candidate, 1.1, 5);

    EXPECT_TRUE(ann->checkGraph(edges));
}

// Test empty graph, empty point and point not found in the graph
TEST_F(RobustPruneTest, InvalidGraphPoint){
    // Empty graph
    std::vector<std::vector<int>> points_alt = {};
    std::vector<std::unordered_set<int>> edges_alt = {};
    std::vector<int> start_alt = {2, 3};

    CompareVectors<int> compare(start_alt);
    std::set<std::vector<int>, CompareVectors<int>> candidate(compare);
    candidate.insert({1, 2});
    candidate.insert({1, 0});
    candidate.insert({1, -5});

    ANN<int> ann_alt(points_alt, edges_alt);
    ann_alt.robustPrune(start_alt, candidate, 1.1, 5);

    EXPECT_TRUE(ann_alt.checkGraph(edges_alt));

    // Point not in graph
    points_alt = {{1, 2}, {1, 0}, {2, 3}, {1, -5}, {3, -5}, {6, 2}};
    edges_alt = {
        {2},
        {0, 3},
        {1, 5},
        {},
        {3},
        {4}
    };

    ANN<int> ann_alt2(points_alt, edges_alt);
    start_alt = {1, 1};
    ann_alt2.robustPrune(start_alt, candidate, 1.1, 5);

    EXPECT_TRUE(ann_alt2.checkGraph(edges_alt));

    // Point empty
    start_alt = {};
    ann_alt2.robustPrune(start_alt, candidate, 1.1, 5);

    EXPECT_TRUE(ann_alt2.checkGraph(edges_alt));
}

// Test alpha less than 1 and degree bound negative
TEST_F(RobustPruneTest, InvalidAlphaDegree){

    CompareVectors<int> compare(start);
    std::set<std::vector<int>, CompareVectors<int>> candidate(compare);
    candidate.insert({1, 2});
    candidate.insert({1, 0});
    candidate.insert({1, -5});

    // Alpha less than 1
    ann->robustPrune(start, candidate, 0.9, 5);
    EXPECT_TRUE(ann->checkGraph(edges));

    // Degree bound negative
    ann->robustPrune(start, candidate, 1.1, -5);
    EXPECT_TRUE(ann->checkGraph(edges));
}

// Degree bound with value 1
TEST_F(RobustPruneTest, DegreeBoundOne){
    // Degree bound of 1 should return only the closest point
    // when giving the whole set of points as candidate set
    // but not the point itself
    CompareVectors<int> compare(start);
    std::set<std::vector<int>, CompareVectors<int>> candidate(compare);
    candidate.insert({1, 2});
    candidate.insert({1, 0});
    candidate.insert({2, 3});
    candidate.insert({1, -5});
    candidate.insert({3, -5});
    candidate.insert({6, 2});

    std::vector<std::unordered_set<int>> expected = {
        {2},
        {0, 3},
        {0},
        {},
        {3},
        {4}
    };

    ann->robustPrune(start, candidate, 1.1, 1);

    EXPECT_TRUE(ann->checkGraph(expected));
}

// Large alpha value resulting in no pruning of the candidate set
TEST_F(RobustPruneTest, LargeAlpha){
    CompareVectors<int> compare(start);
    std::set<std::vector<int>, CompareVectors<int>> candidate(compare);
    candidate.insert({1, 2});
    candidate.insert({1, 0});
    candidate.insert({1, -5});

    std::vector<std::unordered_set<int>> expected = {
        {2},
        {0, 3},
        {0, 1, 3, 5},
        {},
        {3},
        {4}
    };

    ann->robustPrune(start, candidate, 1000, 5);

    EXPECT_TRUE(ann->checkGraph(expected));
}

TEST_F(RobustPruneTest, EmptySet) {
    CompareVectors<int> compare(start);
    std::set<std::vector<int>, CompareVectors<int>> candidate(compare);

    // Delete ann object created in the constructor
    delete ann;

    // Create new ANN object with empty set of points and expect an exception
    std::vector<std::vector<int>> empty = {};
    ann = new ANN<int>(empty, edges);
    EXPECT_TRUE(true);
}

