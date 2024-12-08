#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

// Parse for bin extension
void parseDataVector(const std::string& path, std::vector<float>& vec_with_category_values, std::vector<std::vector<float>>& vec_with_points);
void parseQueryVector(const std::string& path, std::vector<float>& query_filters, std::vector<std::vector<float>>& query_points);

// Parse for fvecs, ivecs and bvecs extensions
template <typename datatype>
std::vector<std::vector<datatype>> parseVecs(const std::string& file_path);

#endif // PARSE_H