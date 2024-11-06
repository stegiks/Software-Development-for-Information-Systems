<h1> Software-Development-for-Information-Systems</h1>

<h2> Team members </h2>

- ***Stefanos Gkikas***  : sdi2100025@di.uoa.gr , 1115202100025
- ***Theodoris Mallios*** : sdi2100090@di.uoa.gr , 1115202100090


<h2> Setting Up </h2>

In order to download the libraries needed for testing, run ```./setup.sh```. For this specific project <b>Googletests</b> was used in order to test the functionality of the modules used.

To download the dataset <b>SiftSmall</b> to test the accuracy of the Vamana Index Algorithm run ```./setup_datasets.sh```.


> <b>WARNING</b> :
If you do not want to download the <b>Googletests</b> library, then run the commmands ```mkdir build``` and ```mkdir bin``` manually to create the required folders for compilation.



<h2> Makefile for Compilation and Running</h2>

The provided Makefile includes several commands to build and run the project:

- <b>Clean the build and binary folders</b>
    
    ```shell
    make clean
    ```

- <b>Build the main executable</b>    
    
    ```shell
    make
    ```
    This compiles the source code and places the executable in the ```./bin``` folder.

- <b>Run the project with default parameters</b>

    ```shell
    make run
    ```
    This command builds the main executable and runs it using the SiftSmall dataset with 100 query vectors to test the Vamana Index Algorithm's accuracy. The default parameters are ```alpha=1, R=50, L=150```.

- <b>Build and run tests</b>

    ```shell
    make test
    ```
    This command compiles all the necessary files and runs the tests that are located in the ```./tests``` folder.

> <b>NOTE</b> : The Makefile rule ```run``` will work only if you have first downloaded the dataset using the ```./setup_datasets.sh``` script provided. If you have not downloaded the dataset, you have to run the executable manually or change <b>```ARGS```</b> variable in the Makefile, with the desired parameters.

> <b>NOTE</b> : If you want to run the project manually, you can use the following command after compiling it : ```./bin/main file_path_base file_path_query extension file_path_gt alpha Regularity L(upper limit)```

<h2>Modules Description</h2>

<h3>ANN (Approximate Nearest Neighbors)</h3>

Source code located in ```./src/ann.cpp``` and header file in ```./include/ann.h```.

The <b>ANN</b> class is the main class that is responsible for the creation of the Vamana Index Algorithm providing all the necessary functions to build the <b>Graph</b>.

<b>Key Components:</b>

- <a id="function_greedy"></a>```greedySearch``` : This function performs a greedy search on the graph to find the k-nearest neighbours of a query vector. This is possible by starting from given node of the graph and maintaining a set of potential nearest neighbors that get pruned when this set exceeds a certain limit. <b>(both involved)</b>

- <a id="function_robust"></a>```robustPrune``` : This function is responsible for pruning the graph by finding the "best" edges for a specific node. It prunes the candidate neighbour set to improve the nearest neighbour graph's quality using ```alpha``` parameter to control the pruning and ```R``` for keeping regularity. <b>(sdi2100090)</b>

- <a id="function_vamana"></a>```Vamana``` : Constructs the approximate nearest neighbour graph implementing the Vamana algorithm with specified parameters ```alpha```, ```R``` and ```L```. <b>(sdi2100090)</b>

- ```calculateMedoid``` : The function calculates the medoid of the dataset given, without making redundant calculations for the same pair of vectors. <b>(sdi2100025)</b>

<b>Design Choices:</b>

- ```Template Programming``` : The use of templates allows the ANN class to handle different types (e.g. int, float, unsigned char) making the implementation flexible for various datasets. This is why we have many explicit instantiations at the end of the ```ann.cpp``` file, so that the compiler can generate the necessary code for the specific types used.

- ```Container Choice``` : For [greedySearch](#function_greedy) function NNS and difference sets are <b>```std::set```</b> type, ensuring that the elements are unique and sorted based on the query vector's distance. Visited set doesn't need to be sorted, so it is a <b>```std::unordered_set```</b> for faster access. The function [robustPrune](#function_robust) uses a <b>```std::set```</b> for the candidate set, which is sorted based again on the vector provided. [Vamana](#function_vamana) function just uses the other functions and the only thing to note is that after returning the Visited set from greedySearch, it is converted from a <b>```std::unordered_set```</b> to a <b>```std::set```</b> for the robustPrune function.

    > <b>NOTE</b> : The containers used handle indexes of the vectors in the dataset, not the actual vectors themselves, to avoid copying the vectors and to save memory. This is why [CompareVectors](#class_compare) functor has a node to point map, to map the indexes to the actual vectors.

<h3>Graph</h3>

Source code located in ```./src/graph.cpp``` and header file in ```./include/graph.h```.

<b>Design Choices:</b>

- ```Adjacency List``` : The graph is implemented as an adjacency list, because it is more memory-efficient than an adjacency matrix. Specifically, the adjacency list is a ```vector of unordered sets```, where each element of the vector corresponds to a node in the graph and contains a hash set of the node's neighbors. This allows for fast insertion and deletion of edges, as well as fast lookup of neighbors. <b>(both involved)</b>

<h3>Utils ANN</h3>

Located in ```./include/utils_ann.h```.

Provides helper functions and and classes for the ANN module.

<b>Key Components:</b>

- <a id="class_compare"></a>```CompareVectors``` : A functor that compares two vectors based on their distance from a query vector. This is used in the ```std::set``` containers to sort the indexes of the vectors based on their distance from the query vector. The distance is calculated using the Euclidean distance formula. <b>(sdi2100025)</b>

- ```VectorHash``` : A simple hash functor that hashes a vector using [FNV-1](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function) algorithm. <b>(sdi2100025)</b> 

<h3>Utils Main</h3>

Source code located in ```./src/utils_main.cpp``` and header file in ```./include/utils_main.h```.

Provides helper functions for the main module.

<b>Key Components:</b>

- ```parseVecs``` : A template function that takes a file path as input and returns a vector of vectors of the specified type. <b>(sdi2100025)</b>

- ```processing``` :  Also a template function that is responsible for calling the Vamana algorithm and then measuring the accuracy of the algorithm. <b>(sdi2100025)</b>

<h3>Tests</h3>

Located in ```./tests``` folder.

Contains 30 tests in total, testing the functionality of all the modules used in the project.

<b>List of Tests:</b>

- ```test_greedy.cpp``` <b>(sdi2100090)</b>

- ```test_robust.cpp``` <b>(sdi2100025)</b>

- ```test_vamana.cpp``` <b>(sdi2100090)</b>

- ```test_graph.cpp``` <b>(sdi2100025)</b>

- ```test_utils_ann.cpp``` <b>(sdi2100090)</b>

- ```test_vector_parser.cpp``` <b>(sdi2100025)</b>
