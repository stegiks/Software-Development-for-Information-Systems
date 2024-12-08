#!/bin/bash
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake ..
make
cd ../..
mkdir build bin data groundtruth graphs