#include "utils_main.h"
#include "ann.h"
#include <iomanip>
#include <filesystem>

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
        std::cerr   << "Error : Base and query files have different extensions" << RESET << std::endl;
        throw std::invalid_argument("Base and query files have different extensions");
        return false;
    }

    if(extension_base != "ivecs" && extension_base != "fvecs" && extension_base != "bvecs" && extension_base != "bin"){
        std::cerr   << "Error : Base vector file has an invalid extension" << RESET << std::endl;
        std::cerr << BLUE << "Valid extensions are : ivecs, fvecs, bvecs, bin" << RESET << std::endl;
        throw std::invalid_argument("Base vector file has an invalid extension");
        return false;
    }

    if(file_format != extension_base){
        std::cerr   << "Error : File format and extension base are not the same" << RESET << std::endl;
        throw std::invalid_argument("File format and extension base are not the same");
        return false;
    }

    if(extension_base != "bin"){
        if(extension_gt != "ivecs"){
            std::cerr   << "Error : Ground truth file has an invalid extension" << RESET << std::endl;
            std::cerr << BLUE << "Valid extension for provided arguments is : ivecs" << RESET << std::endl;
            throw std::invalid_argument("Ground truth file has an invalid extension");
            return false;
        }
    }
    else{
        if(extension_gt != "bin"){
            std::cerr   << "Error : Ground truth file has an invalid extension" << RESET << std::endl;
            std::cerr << BLUE << "Valid extension for provided arguments is : bin" << RESET << std::endl;
            throw std::invalid_argument("Ground truth file has an invalid extension");
            return false;
        }
    }

    return true;
}

// Function that calculates the ground truth vectors for the queries
void calculateGroundTruth(const std::vector<Query>& queries, 
                            const std::vector<float>& base_cat_filter_values, 
                            const std::vector<std::vector<float>>& base_points,
                            std::vector<std::vector<std::pair<float, int>>>& ground_truth){

    // Preallocate memory
    ground_truth.clear();
    ground_truth.resize(queries.size());

    std::size_t n = queries.size();
    for(std::size_t i = 0; i < n; i++){
        const Query& query = queries[i];
        std::vector<std::pair<float, int>> points_for_x_filter;

        // Calculate the distance of the query point to all the points in the base that have the same category value
        // If category value is -1, calculate the distance to all the points in the base
        std::size_t m = base_cat_filter_values.size();
        for(std::size_t j = 0; j < m; j++){
            if(query.category_value == -1 || query.category_value == base_cat_filter_values[j]){
                float distance = calculateDistance(query.point, base_points[j], query.point.size());
                points_for_x_filter.emplace_back(std::make_pair(distance, j));
            }
        }

        // Sort according to the distance
        std::sort(points_for_x_filter.begin(), points_for_x_filter.end(), 
        [](const std::pair<float, int>& a, const std::pair<float, int>& b){ // Lambda function
            return a.first < b.first;
        });

        // Keep maximum 100 points
        int size_gt = (int)std::min(points_for_x_filter.size(), (size_t)100);
        ground_truth[i].assign(points_for_x_filter.begin(), points_for_x_filter.begin() + size_gt);
    }
}

// TODO : Implement the function processBinFormat
void processBinFormat(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, const std::string& file_path_graph, const std::string& algo){
    
    std::vector<std::vector<float>> base;
    std::vector<float> base_category_values;
    parseDataVector(file_path_base, base_category_values, base);

    std::vector<Query> queries;
    parseQueryVector(file_path_query, queries);

    std::vector<std::vector<int>> gt;
    // If the ground truth file is not provided, calculate the ground truth and save it to a file
    if(file_path_gt.empty()){
        std::cout << BLUE << "Calculating ground truth. This may take a while..." << RESET << std::endl;
        std::vector<std::vector<std::pair<float, int>>> temp_gt;
        calculateGroundTruth(queries, base_category_values, base, temp_gt);
        
        std::ofstream file("./groundtruth/groundtruth.bin", std::ios::binary);
        if(!file){
            throw std::runtime_error("Could not open file to save ground truth");
        }

        u_int32_t num_queries = (u_int32_t)queries.size();
        file.write((char*)&num_queries, sizeof(u_int32_t));

        for(const auto& query_gt : temp_gt){
            u_int32_t num_points = (u_int32_t)query_gt.size();
            file.write((char*)&num_points, sizeof(u_int32_t));

            for(const auto& [distance, index] : query_gt){
                file.write((char*)&index, sizeof(u_int32_t));
            }
        }

        file.close();
        parseGroundTruth("./groundtruth/groundtruth.bin", gt);
    }
    else{
        parseGroundTruth(file_path_gt, gt);
    }

    // Check if the files are parsed successfully
    if(base.empty() || queries.empty() || gt.empty()){
        throw std::runtime_error("Parsing failed and returned empty vectors");
    }

    std::cout << GREEN << "Files parsed successfully" << RESET << std::endl;

    // Init ANN class and run Vamana algorithm
    ANN<float> ann(base, base_category_values);

    // Open the file to write the graph
    if(file_path_graph.empty()){
        std::cout << BLUE << "Running filtered Vamana algorithm to create the graph" << RESET << std::endl;
        if(algo == "stitch"){
            ann.stitchedVamana(alpha, L, R, 100);
            std::cout << GREEN << "Stitched Vamana Graph executed successfully" << RESET << std::endl;
        }
        else{
            ann.filteredVamana(alpha, L, R, 5);
            std::cout << GREEN << "Filtered Vamana Graph executed successfully" << RESET << std::endl;
        }
    }
    else{
        // Load the graph from the file
        ann.loadGraph(file_path_graph);
        std::cout << GREEN << "Graph loaded successfully" << RESET << std::endl;

        // Run filtered find medoid to fill filter_to_start_node used in filteredGreedySearch
        ann.filteredFindMedoid(5);
    }

    int total_correct_guesses = 0;
    int total_gt_size = 0;

    for(std::size_t i = 0; i < queries.size(); i++){
        int k = (int)gt[i].size();

        CompareVectors<float> compare(ann.node_to_point_map, queries[i].point);
        std::set<int, CompareVectors<float>> NNS(compare);
        std::unordered_set<int> Visited;

        // Call filtered Greedy
        if(queries[i].query_id == 0){
            ann.filteredGreedySearch(-1, k, L, queries[i].category_value, NNS, Visited, compare);
        }
        else{
            int start_node = ann.getStartNode(queries[i].category_value);
            if(start_node == -1) continue;
            ann.filteredGreedySearch(start_node, k, L, queries[i].category_value, NNS, Visited, compare);
        }

        // Search in the ground truth
        int correct = 0;
        for(const int& index : gt[i]){
            if(NNS.find(index) != NNS.end()){
                correct++;
            }
        }
        
        float recall = (float)correct / k * 100;
        std::cout << "Query " << i << " with category value " << queries[i].category_value << " recall : " << recall << "%" << std::endl;
        
        total_correct_guesses += correct;
        total_gt_size += k;
    }

    float total_recall = (float)total_correct_guesses / total_gt_size * 100;
    std::cout << BLUE << "Total recall : " << RESET << total_recall << "%" << std::endl;

    if (file_path_graph.empty()) {
        std::ostringstream file_name_stream;
        std::string algorithm_used = algo == "stitch" ? "stitched" : "filtered";
        file_name_stream << "./graphs/graph_" << algorithm_used << "_"
                        << std::fixed << std::setprecision(3)
                        << alpha << "_" << R << "_" << L;

        std::string file_name = file_name_stream.str();

        // Check if the file already exists
        if (std::filesystem::exists(file_name)) {
            std::cout << YELLOW << "File already exists: " << file_name << RESET << std::endl;
        } else {
            ann.saveGraph(file_name);
            std::cout << GREEN << "Graph saved successfully: " << file_name << RESET << std::endl;
        }
    }

}

template <typename datatype>
void processVecFormat(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, const std::string& file_path_graph){
    
    std::vector<std::vector<datatype>> base = parseVecs<datatype>(file_path_base);
    std::vector<std::vector<datatype>> query = parseVecs<datatype>(file_path_query);
    std::vector<std::vector<int>> gt = parseVecs<int>(file_path_gt);

    // Check if the files are parsed successfully
    if(base.empty() || query.empty() || gt.empty()){
        throw std::runtime_error("Parsing failed and returned empty vectors");
    }

    std::cout << GREEN << "Files parsed successfully" << RESET << std::endl;

    // Init ANN class and run Vamana algorithm
    ANN<datatype> ann(base, (size_t)R);
    std::cout << GREEN << "ANN class initialized successfully" << RESET << std::endl;
    
    // Open the file to write the graph
    if(file_path_graph.empty()){
        std::cout << BLUE << "Running Vamana algorithm to create the graph" << RESET << std::endl;
        ann.Vamana(alpha, L, R, true);
        std::cout << GREEN << "Vamana Graph executed successfully" << RESET << std::endl;
    }
    else{
        // Load the graph from the file
        ann.loadGraph(file_path_graph);
        std::cout << GREEN << "Graph loaded successfully" << RESET << std::endl;
    }

    // For every query point, find the results and compare with ground truth
    int total_correct_guesses = 0;
    int total_gt_size = 0;

    for(std::size_t i = 0; i < query.size(); i++){
        int k = (int)gt[i].size();

        CompareVectors<datatype> compare(ann.node_to_point_map, query[i]);
        std::set<int, CompareVectors<datatype>> NNS(compare);
        std::unordered_set<int> Visited;

        // Call Greedy search to find the nearest neighbours
        ann.greedySearch(ann.getMedoid(), k, L, NNS, Visited, compare);

        // Search in the ground truth
        int correct = 0;
        for(const int& index : gt[i]){
            if(NNS.find(index) != NNS.end()){
                correct++;
            }
        }
        float recall = (float)correct / k * 100;
        std::cout << "Query " << i << " recall : " << recall << "%" << std::endl;

        total_correct_guesses += correct;
        total_gt_size += k; 
    }

    float total_recall = (float)total_correct_guesses / total_gt_size * 100;
    std::cout << BLUE << "Total recall : " << RESET << total_recall << "%" << std::endl;

    if (file_path_graph.empty()) {
        std::ostringstream file_name_stream;
        file_name_stream << "./graphs/graph_"
                        << std::fixed << std::setprecision(3)
                        << alpha << "_" << R << "_" << L;

        std::string file_name = file_name_stream.str();

        // Check if the file already exists
        if (std::filesystem::exists(file_name)) {
            std::cout << YELLOW << "File already exists: " << file_name << RESET << std::endl;
        } else {
            ann.saveGraph(file_name);
            std::cout << GREEN << "Graph saved successfully: " << file_name << RESET << std::endl;
        }
    }
}

// Explicit instantiation of the processing function
template void processVecFormat<int>(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, const std::string& file_path_graph);
template void processVecFormat<float>(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, const std::string& file_path_graph);
template void processVecFormat<unsigned char>(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, const std::string& file_path_graph);