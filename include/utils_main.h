#ifndef UTILS_MAIN_H
#define UTILS_MAIN_H

#include <iostream>
#include <string>
#include "defs.h"
#include "parse.h"

// Function to find the extension of a file
std::string findExtension(const std::string& file_path);

// Function to validate the extension of the files
bool validateExtension(const std::string& extension_base, const std::string& extension_query, const std::string& extension_gt, const std::string& file_format);

// Calculate the ground truth for the given query
void calculateGroundTruth(const std::vector<Query>& queries, const std::vector<float>& base_cat_filter_values, const std::vector<std::vector<float>>& base_points, std::vector<std::vector<std::pair<float, int>>>& ground_truth);

// Process files with bin format and run the Vamana algorithm
void processBinFormat(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, const std::string& file_path_graph, const std::string& algo);

// Process files with vec format and run the Vamana algorithm
template <typename datatype>
void processVecFormat(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, const std::string& file_path_graph);

#endif // utils.h