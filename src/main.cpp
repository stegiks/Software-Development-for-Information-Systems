#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <set>
#include "defs.h"
#include "utils_main.h"
#include "ann.h"

void printHelp(){
    std::cout << GREEN << "Usage: " << std::endl << RESET 
              << CYAN << "./main " << RESET
              << YELLOW << "-b " << MAGENTA << "<file_path_base> " << RESET
              << YELLOW << "-q " << MAGENTA << "<file_path_query> " << RESET
              << YELLOW << "-f " << MAGENTA << "<extension> " << RESET
              << "[" << YELLOW << "-gt " << MAGENTA << "<file_path_gt>" << RESET << "] "
              << YELLOW << "-a " << MAGENTA << "<alpha> " << RESET
              << YELLOW << "-R " << MAGENTA << "<Regularity> " << RESET
              << YELLOW << "-L " << MAGENTA << "<L(upper limit)> " << RESET
              << "[" << YELLOW << "-load " << MAGENTA << "<file_path_graph>" << RESET << "] "
              << "[" << YELLOW << "-save " << MAGENTA << "<file_path_graph>" << RESET << "] "
              << "[" << YELLOW << "-algo " << MAGENTA << "<algorithm>" << RESET << "] "
              << "[" << YELLOW << "-query" << MAGENTA << "<y/n>" << RESET << "]"
              << std::endl << std::endl;

    std::cout << GREEN << "Options:" << RESET << std::endl;
    std::cout  << "  -b " << "<file_path_base> " 
              << ": Path to the base vectors file." << std::endl;
    std::cout << "  -q " << "<file_path_query> " 
              << ": Path to the query vectors file." << std::endl;
    std::cout << "  -f " << "<extension> " 
              << ": File format extension (e.g., fvecs, bin)." << std::endl;
    std::cout << "  -gt " << "<file_path_gt> " 
              << ": (Optional) Path to ground truth vectors file." << std::endl;
    std::cout << "  -a " << "<alpha> " 
              << ": Alpha parameter for search." << std::endl;
    std::cout << "  -R " << "<Regularity> " 
              << ": Regularity parameter (R)." << std::endl;
    std::cout << "  -L " << "<L(upper limit)> " 
              << ": Upper limit (L) for search." << std::endl;
    std::cout << "  -load " << "<file_path_graph> " 
              << ": (Optional) Path to precomputed graph file." << std::endl;
    std::cout << "  -save " << "<file_path_graph> "
                << ": (Optional) Path to save the computed graph." << std::endl;
    std::cout << "  -algo " << "stitch/filter " 
              << ": (Optional) Algorithm to use for filtered datasets. Default is FilteredVamana." << std::endl;
    std::cout << "  -query " << "y/n "
              << ": (Optional) Flag to enable (y) or disable (n) query execution. Default is y. NOTE: This flag is overridden if a graph file is provided." << std::endl << std::endl;

    std::cout << GREEN << "Example:" << RESET << std::endl;
    std::cout << CYAN << "  ./main -b base.bin -q query.bin -f bin -a 1.1 -R 10 -L 100 -query y" << RESET << std::endl;
}

void printUsage() {
    std::cerr << "Wrong usage of the program. Find more information with ./bin/main -h or ./bin/main --help" << std::endl;
}

std::map<std::string, std::string> parseArguments(int argc, char** argv) {
    if ((argc - 1) % 2 != 0) {  // Ensure flags and values are paired
        throw std::invalid_argument("Invalid number of arguments. Flags must have values.");
    }

    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; i += 2) {
        std::string flag = argv[i];
        if (flag[0] != '-') {  // Ensure the flag starts with a '-'
            throw std::invalid_argument("Invalid flag format: " + flag);
        }

        std::string value = argv[i + 1];
        args[flag] = value;  // Map the flag to its value
    }

    return args;
}

void checkRequiredFlags(const std::map<std::string, std::string>& args, const std::set<std::string>& requiredFlags) {
    for (const auto& flag : requiredFlags) {
        if (args.find(flag) == args.end()) {
            throw std::invalid_argument("Missing required flag: " + flag);
        }
    }
}

int main(int argc, char** argv) {
    try {
        if( argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") ){
            printHelp();
            return 0;
        }

        if (argc < 13) {  // Minimum required arguments: 7 flags, each with a value
            printUsage();
            throw std::invalid_argument("Not enough arguments");
        }

        auto args = parseArguments(argc, argv);

        // Define the set of required flags
        std::set<std::string> requiredFlags = { "-b", "-q", "-f", "-a", "-R", "-L" };

        // Check if all required flags are present
        checkRequiredFlags(args, requiredFlags);

        // Extract arguments
        std::string file_path_base = args["-b"];
        std::string file_path_query = args["-q"];
        std::string file_format = args["-f"];
        float alpha = std::stof(args["-a"]);
        int R = std::stoi(args["-R"]);
        int L = std::stoi(args["-L"]);
        bool do_query = true;

        if(args.find("-query") != args.end()){
            std::string query_flag = args["-query"];
            if(query_flag == "n"){
                do_query = false;
            }
            else if(query_flag != "y"){
                throw std::invalid_argument("Invalid query flag");
            }
        }

        // Check optional flags
        std::string file_path_gt = "";
        if (args.find("-gt") != args.end()) {
            file_path_gt = args["-gt"];
        }

        std::string file_path_load = "";
        if (args.find("-load") != args.end()) {
            file_path_load = args["-load"];

            if(!do_query){
                std::cout << BLUE << "Overriding -query flag because a graph file was provided" << RESET << std::endl;
                do_query = true;
            }
        }

        std::string file_path_save = "";
        if (args.find("-save") != args.end()) {
            file_path_save = args["-save"];
        }

        // Validate extension
        if (!validateExtension(file_path_base, file_path_query, file_path_gt, file_format)) {
            throw std::invalid_argument("Invalid extension");
        }

        // Call processing function based on the file format
        if (file_format == "fvecs") {
            processVecFormat<float>(file_path_base, file_path_query, file_path_gt,
            alpha, R, L, file_path_load, file_path_save, do_query);
        }
        else if (file_format == "ivecs") {
            processVecFormat<int>(file_path_base, file_path_query, file_path_gt,
            alpha, R, L, file_path_load, file_path_save, do_query);
        }
        else if (file_format == "bvecs") {
            processVecFormat<unsigned char>(file_path_base, file_path_query, file_path_gt, 
            alpha, R, L, file_path_load, file_path_save, do_query);
        }
        else if (file_format == "bin") {
            processBinFormat(file_path_base, file_path_query, file_path_gt,
            alpha, R, L, file_path_load, file_path_save, args["-algo"], do_query);
        }
        else {
            std::cerr << RED << "Error : Invalid extension" << RESET << std::endl;
            printUsage();
            throw std::invalid_argument("Invalid extension");
        }

        std::cout << GREEN << "Processing completed successfully" << RESET << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << RED << "Error : " << e.what() << RESET << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << RED << "Error : " << e.what() << RESET << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << RED << "Error : Unknown exception" << RESET << std::endl;
        return 1;
    }

    return 0;
}
