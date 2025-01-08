
# Shory
## ann.cpp
## calculateMedoid 488-494 Possibly parallelize the calculation of distances DONE

## stitchedVamana 635-659 Parallelize the for loop since each filtere is independent, SOS the addEdge calls to the main graph could potentially be CS DONE
## stitchedVamana 664-674 Parallelize the loop by changing the structure of the for loop; instead of iterating over all the points iterate first over all the filters and get the points of each filter DONE
## filteredVamana 700 Parallellize the loop by changing the structure of the for loop to have two loops with the outside being the filters

# Tsefo
## ann.cpp
## Vamana 577 Check whether robustPrune will make iterating the loop not feasible DONE
## filteredFindMedoid 452-465 parallelize loop with care for the indexing whether or not it is CS DONE
## find how to precalculate distances between points before building the graph

## graph.cpp
## enforceRegular 118-142 parallelize the for loop with private lists to resolve the CS


# Check whether they can be parallelized or they are called insidie a parallelization

## check for pruneSet more thoroughly
## filteredGreedySearch 284-295 Parallelize the neighbours insertion into NNS and Diff sets with critical section the insertion into the sets
## greedySearch 338-343 change for loop structure to be like filteredGreedySearch and parallelize
## robustPrune 394-415 change the way iteration is performed inorder to parallellize with critical section the removal from candidate_set
## filteredGreedySearch 246 check whether the initialization of the set can be parallelized(Tasos airlines)
## filteredGreedySearch 253-262 Parallelize the GreedySearch calls maybe with omp for nowait and critical section
## greedySearch 321 check whether the initialization of the set here can also be parallelized
## utils_ann.h
## calculateDistance 33-39 parallelize the distance calculation across dimensions




