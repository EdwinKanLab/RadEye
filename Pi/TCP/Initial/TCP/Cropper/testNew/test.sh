#!/bin/bash


mkdir test_build
cd test_build
cmake ..
make
./CropperTest
cd ..
rm -r test_build