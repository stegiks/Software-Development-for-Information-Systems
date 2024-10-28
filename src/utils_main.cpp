#include "utils_main.h"

std::string findExtension(const std::string& file_path){
    std::size_t pos = file_path.find_last_of(".");
    if(pos == std::string::npos){
        return "";
    }
    return file_path.substr(pos + 1);
}

bool validateExtension(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, const std::string& file_format){
    
    std::string extension_base = findExtension(file_path_base);
    std::string extension_query = findExtension(file_path_query);
    std::string extension_gt = findExtension(file_path_gt);
    
    if(extension_base != extension_query){
        std::cerr << RED << "Error : Base and query files have different extensions" << RESET << std::endl;
        return false;
    }

    if(extension_base != "ivecs" && extension_base != "fvecs" && extension_base != "bvecs"){
        std::cerr << RED << "Error : Base and Query files have an invalid extension" << RESET << std::endl;
        std::cerr << BLUE << "Valid extensions are : ivecs, fvecs, bvecs" << RESET << std::endl;
        return false;
    }

    if(file_format != extension_base){
        std::cerr << RED << "Error : File format and extension base are not the same" << RESET << std::endl;
        return false;
    }

    if(extension_gt != "ivecs"){
        std::cerr << RED << "Error : Ground truth file has an invalid extension" << RESET << std::endl;
        std::cerr << BLUE << "Valid extension is : ivecs" << RESET << std::endl;
        return false;
    }

    return true;
}

// template <typename datatype>
// void processing(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_format, const std::string& file_path_gt, float alpha, int R, int L){
// }