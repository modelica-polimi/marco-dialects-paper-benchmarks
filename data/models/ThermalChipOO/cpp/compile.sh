#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

# C++ code size
CPP_SIZE_FILE=$STATS_DIR/cpp-cpp-size-$EXPERIMENT.txt
find "$path" -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cpp" \) -print0 | du -scb --files0-from=- | tail -n1 > $CPP_SIZE_FILE

# Binary

# Compile the model
cmake -S $path -B $BUILD_DIR -DCMAKE_BUILD_TYPE=Release

COMPILATION_TIME_FILE=$STATS_DIR/cpp-compilation-time-$EXPERIMENT.txt
/usr/bin/time -p -a -o $COMPILATION_TIME_FILE cmake --build $BUILD_DIR $COMPILATION_ARGS -- -j || exit 1

# Get the binary size
BINARY_SIZE_FILE=$STATS_DIR/cpp-binary-size-$EXPERIMENT.txt
wc -c $BUILD_DIR/simulation > $BINARY_SIZE_FILE
