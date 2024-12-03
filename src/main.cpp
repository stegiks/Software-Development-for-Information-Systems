#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <set>
#include "defs.h"
#include "utils_main.h"
#include "ann.h"

void printUsage() {
    std::cerr << BLUE << "Usage : " << RESET 
              << "./main -b <file_path_base> -q <file_path_query> -f <extension> "
              << "-gt <file_path_gt> -a <alpha> -R <Regularity> -L <L(upper limit)> [-load <file_path_graph>]" 
              << std::endl;
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
        if (argc < 15) {  // Minimum required arguments: 7 flags, each with a value
            std::cerr << RED << "Error : Not enough arguments" << RESET << std::endl;
            printUsage();
            throw std::invalid_argument("Not enough arguments");
        }

        auto args = parseArguments(argc, argv);

        // Define the set of required flags
        std::set<std::string> requiredFlags = {"-b", "-q", "-f", "-gt", "-a", "-R", "-L"};

        // Check if all required flags are present
        checkRequiredFlags(args, requiredFlags);

        // Extract arguments
        std::string file_path_base = args["-b"];
        std::string file_path_query = args["-q"];
        std::string file_format = args["-f"];
        std::string file_path_gt = args["-gt"];
        float alpha = std::stof(args["-a"]);
        int R = std::stoi(args["-R"]);
        int L = std::stoi(args["-L"]);


        // Check if optional flag is present
        std::string file_path_graph = "";
        if (args.find("-load") != args.end()) {
            file_path_graph = args["-load"];
        }

        // Validate extension
        if (!validateExtension(file_path_base, file_path_query, file_path_gt, file_format)) {
            throw std::invalid_argument("Invalid extension");
        }

        // Call processing function based on the file format
        if (file_format == "fvecs") {
            processing<float>(file_path_base, file_path_query, file_path_gt, alpha, R, L, file_path_graph);
        } else if (file_format == "ivecs") {
            processing<int>(file_path_base, file_path_query, file_path_gt, alpha, R, L, file_path_graph);
        } else if (file_format == "bvecs") {
            processing<unsigned char>(file_path_base, file_path_query, file_path_gt, alpha, R, L, file_path_graph);
        } else {
            std::cerr << RED << "Error : Invalid extension" << RESET << std::endl;
            printUsage();
            throw std::invalid_argument("Invalid extension");
        }

        std::cout << GREEN << "Processing completed successfully" << RESET << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << RED << "Error : " << e.what() << RESET << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << RED << "Error : " << e.what() << RESET << std::endl;
        return 1;
    } catch (...) {
        std::cerr << RED << "Error : Unknown exception" << RESET << std::endl;
        return 1;
    }

    return 0;
}
