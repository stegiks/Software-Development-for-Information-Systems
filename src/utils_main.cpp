#include "utils_main.h"

std::string findExtension(const std::string& file_path){
    std::size_t pos = file_path.find_last_of(".");
    if(pos == std::string::npos){
        return "";
    }
    return file_path.substr(pos + 1);
}

bool validateExtension(const std::string& extension_base, const std::string& extension_query, const std::string& extension_gt){
    if(extension_base != extension_query){
        std::cerr << RED << "Error : Base and query files have different extensions" << RESET << std::endl;
        return false;
    }

    if(extension_base != "ivecs" && extension_base != "fvecs" && extension_base != "bvecs"){
        std::cerr << RED << "Error : Base and Query files have an invalid extension" << RESET << std::endl;
        std::cerr << BLUE << "Valid extensions are : ivecs, fvecs, bvecs" << RESET << std::endl;
        return false;
    }

    if(extension_gt != "ivecs"){
        std::cerr << RED << "Error : Ground truth file has an invalid extension" << RESET << std::endl;
        std::cerr << BLUE << "Valid extension is : ivecs" << RESET << std::endl;
        return false;
    }

    return true;
}