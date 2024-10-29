#include "defs.h"
#include "utils_main.h"
#include "ann.h"

int main(int argc, char** argv){

    if(argc < 8){
        std::cout << RED << "Error : Not enough arguments" << RESET << std::endl;
        std::cout << BLUE << "Usage : "  << RESET << "./main <file_path_base> <file_path_query> <extension> <file_path_gt> <alpha> <Regularity> <L(upper limit)>" << std::endl;
        return 1;
    }

    std::string file_path_base = argv[1];
    std::string file_path_query = argv[2];
    std::string file_format = argv[3];
    std::string file_path_gt = argv[4];
    float alpha = std::stof(argv[5]);
    int R = std::stoi(argv[6]);
    int L = std::stoi(argv[7]);

    if(!validateExtension(file_path_base, file_path_base, file_path_gt, file_format))
        return 1;
    
    if(file_format == "fvecs"){
        processing<float>(file_path_base, file_path_query, file_path_gt, alpha, R, L);
    }
    else if(file_format == "ivecs"){
        processing<int>(file_path_base, file_path_query, file_path_gt, alpha, R, L);
    }
    else if(file_format == "bvecs"){
        processing<unsigned char>(file_path_base, file_path_query, file_path_gt, alpha, R, L);
    }
    else{
        std::cout << RED << "Error : Invalid extension" << RESET << std::endl;
        std::cout << BLUE << "Valid extensions are : ivecs, fvecs, bvecs" << RESET << std::endl;
        return 1;
    }

    std::cout << GREEN << "Vamana Graph created successfully" << RESET << std::endl;

    return 0;
}