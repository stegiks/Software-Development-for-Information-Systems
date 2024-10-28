#ifndef UTILS_MAIN_H
#define UTILS_MAIN_H

#include <iostream>
#include <string>
#include "defs.h"

// Function to find the extension of a file
std::string findExtension(const std::string& file_path);

// Function to validate the extension of the files
bool validateExtension(const std::string& extension_base, const std::string& extension_query, const std::string& extension_gt, const std::string& file_format);

// Main function that processes the input arguments and makes the Vamana Graph
// template <typename datatype>
// void processing(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_format, const std::string& file_path_gt, float alpha, int R, int L);

#endif // utils.h