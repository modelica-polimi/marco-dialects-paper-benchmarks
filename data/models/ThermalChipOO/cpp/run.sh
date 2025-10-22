#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

export BUILD_DIR=$MODEL_BUILD_ROOT/cpp
export LOG_DIR=$MODEL_LOG_ROOT/cpp
export RESULTS_DIR=$MODEL_RESULTS_ROOT/cpp
export STATS_DIR=$MODEL_STATS_ROOT/cpp

mkdir -p $BUILD_DIR
mkdir -p $LOG_DIR
mkdir -p $RESULTS_DIR
mkdir -p $STATS_DIR

# Euler Forward

export EXPERIMENT_NAME="euler-forward"
export COMPILATION_ARGS=""
export SIMULATION_ARGS="model=ThermalChipSimpleBoundaryEulerExp stop_time=0.4 num_intervals=266667"

"$path/compile_simulate_all.sh"

rm -rf $BUILD_DIR
