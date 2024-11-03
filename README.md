<h1> Software-Development-for-Information-Systems</h1>

<h2> Team members </h2>

- ***Stefanos Gkikas***  : sdi2100025
- ***Theodoris Mallios*** : sdi2100090


<h2> Setting Up </h2>

In order to download the libraries needed for testing, run ```./setup.sh```. For this specific project <b>Googletests</b> was used in order to test the functionality of the modules used.

To download the dataset <b>SiftSmall</b> to test the accuracy of the Vamana Index Algorithm run ```./setup_datasets.sh```.


> <b>WARNING</b>
If you do not want to download the <b>Googletests</b> library, then run the commmands ```mkdir build``` and ```mkdir bin``` manually to create the required folders.



<h2> Makefile and Running</h2>

The Makefile provided has the following commands:
```make clean```: clean the binary and build folders
```make```: Build the ```main``` binary in the ```./bin``` folder used for testing datasets
```make test```: build and run the tests for the modules *GreedySearch, RobustPrune, VamanaIndexing, Graph, VectorParser, Utils*
```make run```: Build the ```main``` binary and run it for the <b>SiftSmall</b> dataset that has 10k base vectors and uses 100 query vectors to test the accuracy of the VamanaIndexing Algorithm with parameters: *alpha=1, R=50, L=150*

<h2>Modules</h2>
<b>Approximating Nearest Nei