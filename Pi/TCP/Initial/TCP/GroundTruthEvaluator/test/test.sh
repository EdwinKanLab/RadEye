#!/bin/bash


mkdir test_build
cd test_build
cmake ..
make
./GroundTruthEvaluatorTest
cd ..
# rm -r test_build