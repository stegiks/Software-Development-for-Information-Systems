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
template <typename datatype>
void calculateGroundTruth(const std::vector<std::vector<datatype>>& queries, const std::vector<std::vector<datatype>>& base_points, std::vector<std::vector<std::pair<float, int>>>& ground_truth, const std::vector<float>* query_category_values = nullptr, const std::vector<float>* base_category_values = nullptr);

// Process files with bin format and run the Vamana algorithm
void processBinFormat(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, const std::string& file_path_load, const std::string& file_path_save, const std::string& algo, bool do_query, const std::string& file_path_log);

// Process files with vec format and run the Vamana algorithm
template <typename datatype>
void processVecFormat(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, const std::string& file_path_load, const std::string& file_path_save, bool do_query, const std::string& file_path_log);

#endif // utils.h