#!/bin/bash


mkdir test_build
cd test_build
cmake ..
make
./WarperTest
cd ..
rm -r test_build
