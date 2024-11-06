#include <gtest/gtest.h>
#include "utils_ann.h"

TEST(UtilsANN, BasicHashFunctionality){
    std::vector<int> v1 = {1, 2, 3};
    std::vector<int> v2 = {1, 2, 3};
    std::vector<int> v3 = {1, 2, 4};
    std::vector<int> v4 = {1, 2, 3, 4};
    std::vector<int> v5 = {1, 2, 3, 4, 5};

    // Same vectors should hash to the same value
    // and different vectors should hash to a different one.
    VectorHash<int> vector_hasher;
    EXPECT_EQ(vector_hasher(v1), vector_hasher(v2));
    EXPECT_NE(vector_hasher(v1), vector_hasher(v3));
    EXPECT_NE(vector_hasher(v1), vector_hasher(v4));
    EXPECT_NE(vector_hasher(v1), vector_hasher(v5));
}

TEST(UtilsANN, DistanceTest){
    std::vector<float> v1 = {3.0, 4.0};
    std::vector<float> v2 = {6.0, 8.0};

    // Test known distance
    EXPECT_FLOAT_EQ(calculateDistance(v1, v2), 5.0);

    // Test zero distance
    std::vector<float> v3 = {3.0, 4.0};
    EXPECT_FLOAT_EQ(calculateDistance(v1, v3), 0.0);
}

TEST(UtilsANN, VectorComparison){
    std::vector<int> base_vec = {10, 20, 30};

    std::vector<int> v1 = {15, 25, 35};
    std::vector<int> v2 = {4, 25, 35};

    std::vector<std::vector<int>> vec = {v1, v2};

    CompareVectors<int> vector_comparator(vec, base_vec);
    std::set<int, CompareVectors<int>> custom_set(vector_comparator);
    custom_set.insert(0);
    custom_set.insert(1);

    // v1(0) should be closer to base_vec than v2(1)
    EXPECT_EQ(*(custom_set.begin()), 0);
}
