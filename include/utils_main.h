#ifndef UTILS_MAIN_H
#define UTILS_MAIN_H

#include <iostream>
#include <string>
#include "defs.h"

template <typename datatype>
struct Query{
    float query_id;
    float category_value;
    std::vector<datatype> point;
};

// Function to find the extension of a file
std::string findExtension(const std::string& file_path);

// Function to validate the extension of the files
bool validateExtension(const std::string& extension_base, const std::string& extension_query, const std::string& extension_gt, const std::string& file_format);

// Function to parse the base vectors for filtered search
template <typename datatype>
void parseDataVector(const std::string& path, std::vector<float>& vec_with_category_values, std::vector<std::vector<datatype>>& vec_with_points);

// Function to parse query vectors for filtered search
template <typename datatype>
void parseQueryVector(const std::string& path, std::vector<Query<datatype>>& queries);

// Function to parse vectors from a file
template <typename datatype>
std::vector<std::vector<datatype>> parseVecs(const std::string& file_path);

// Main function that processes the input arguments and makes the Vamana Graph
template <typename datatype>
void processing(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L);

#endif // utils.h