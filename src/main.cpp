#include "defs.h"
#include "utils_main.h"
#include "ann.h"

int main(int argc, char** argv){
    try{
        if(argc < 8){
            std::cerr << RED << "Error : Not enough arguments" << RESET << std::endl;
            std::cerr << BLUE << "Usage : "  << RESET << "./bin/main <file_path_base> <file_path_query> <extension> <file_path_gt> <alpha> <Regularity> <L(upper limit)>" << std::endl;
            throw std::invalid_argument("Not enough arguments");
        }

        std::string file_path_base = argv[1];
        std::string file_path_query = argv[2];
        std::string file_format = argv[3];
        std::string file_path_gt = argv[4];
        float alpha = std::stof(argv[5]);
        int R = std::stoi(argv[6]);
        int L = std::stoi(argv[7]);

        if(!validateExtension(file_path_base, file_path_base, file_path_gt, file_format)){
            throw std::invalid_argument("Invalid extension");
        }

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
            std::cerr << RED << "Error : Invalid extension" << RESET << std::endl;
            std::cerr << BLUE << "Valid extensions are : ivecs, fvecs, bvecs" << RESET << std::endl;
            throw std::invalid_argument("Invalid extension");
            return 1;
        }

        std::cout << GREEN << "Processing completed successfully" << RESET << std::endl;
    }
    catch(const std::invalid_argument& e){
        std::cerr << RED << "Error : " << e.what() << RESET << std::endl;
        return 1;
    }
    catch(const std::out_of_range& e){
        std::cerr << RED << "Error : " << e.what() << RESET << std::endl;
        return 1;
    }
    catch(const std::runtime_error& e){
        std::cerr << RED << "Error : " << e.what() << RESET << std::endl;
        return 1;
    }
    catch(const std::exception& e){
        std::cerr << RED << "Error : " << e.what() << RESET << std::endl;
        return 1;
    }
    catch(...){
        std::cerr << RED << "Error : Unknown exception" << RESET << std::endl;
        return 1;
    }

    return 0;
}