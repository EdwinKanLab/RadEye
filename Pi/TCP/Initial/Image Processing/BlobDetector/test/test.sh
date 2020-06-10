#!/bin/bash


mkdir test_build
cd test_build
cmake ..
make
./BlobDetectorTest
cd ..
rm -r test_build