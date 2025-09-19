#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

export BUILD_DIR=$MODEL_BUILD_ROOT/marco
export LOG_DIR=$MODEL_LOG_ROOT/marco
export RESULTS_DIR=$MODEL_RESULTS_ROOT/marco
export STATS_DIR=$MODEL_STATS_ROOT/marco

mkdir -p $BUILD_DIR
mkdir -p $LOG_DIR
mkdir -p $RESULTS_DIR
mkdir -p $STATS_DIR

# Euler Forward

export EXPERIMENT_NAME="euler-forward"
export COMPILATION_ARGS="--solver=euler-forward -Xmarco -equations-runtime-scheduling"
export SIMULATION_ARGS="--time-step=0.0000015 --end-time=0.4"

"$path/compile_simulate_all.sh"

# IDA

export EXPERIMENT_NAME="ida"
export COMPILATION_ARGS="--solver=ida -Xmarco -equations-runtime-scheduling"
export SIMULATION_ARGS="--end-time=0.4"

"$path/compile_simulate_all.sh"

rm -rf $BUILD_DIR
