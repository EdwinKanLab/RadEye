#!/bin/bash


mkdir test_build
cd test_build
cmake ..
make
./CornerDetectorTest
cd ..
rm -r test_build