#!/bin/sh

rm -rf bin > /dev/null 2>&1;
echo "Testing using TSAN.";
cmake -D ENABLE_TSAN=TRUE -Bbin > /dev/null 2>&1;
cmake --build bin > /dev/null 2>&1;
./bin/test_julia_set;

rm -rf bin > /dev/null 2>&1;
echo "Testing using UBSAN, ASAN.";
cmake -D ENABLE_UBSAN=TRUE -D ENABLE_ASAN=TRUE -Bbin > /dev/null 2>&1;
cmake --build bin > /dev/null 2>&1;
./bin/test_julia_set;
