#include <gtest/gtest.h>
#include "ann.h"

TEST(FilteredFindMedoid, BasicFunctionality){
    std::vector<std::vector<int>> points = {
        {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18},
        {19, 20, 21}, {22, 23, 24}, {25, 26, 27}, {28, 29, 30}, {31, 32, 33}, {34, 35, 36},
        {37, 38, 39}, {40, 41, 42}, {43, 44, 45}, {46, 47, 48}, {49, 50, 51}, {52, 53, 54}
    };

    std::vector<float> filters = {
        1.0f, 2.0f, 1.0f, 3.0f, 1.0f, 2.0f,
        3.0f, 1.0f, 1.0f, 2.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 1.0f, 2.0f, 3.0f, 4.0f
    };

    ANN<int> ann(points, filters);
    int tau = 2;
    ann.filteredFindMedoid(tau);
    EXPECT_TRUE(ann.checkFilteredFindMedoid(5));
}