#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

// Queries with category values
struct Query{
    float query_id;
    float category_value;
    std::vector<float> point;
};

// Parse for bin extension
void parseDataVector(const std::string& path, std::vector<float>& vec_with_category_values, std::vector<std::vector<float>>& vec_with_points);
void parseQueryVector(const std::string& path, std::vector<Query>& queries);
void parseGroundTruth(const std::string& path, std::vector<std::vector<int>>& ground_truth);

// Parse for fvecs, ivecs and bvecs extensions
template <typename datatype>
std::vector<std::vector<datatype>> parseVecs(const std::string& file_path);

#endif // PARSE_H