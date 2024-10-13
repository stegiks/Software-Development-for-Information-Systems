#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include "defs.h"

// Function to find the extension of a file
std::string findExtension(const std::string& file_path);

// Function to validate the extension of the files
bool validateExtension(const std::string& extension_base, const std::string& extension_query, const std::string& extension_gt);
#endif // utils.h