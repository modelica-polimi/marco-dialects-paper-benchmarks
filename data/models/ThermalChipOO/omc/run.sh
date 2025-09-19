#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

export BUILD_DIR=$MODEL_BUILD_ROOT/omc
export LOG_DIR=$MODEL_LOG_ROOT/omc
export RESULTS_DIR=$MODEL_RESULTS_ROOT/omc
export STATS_DIR=$MODEL_STATS_ROOT/omc

mkdir -p $BUILD_DIR
mkdir -p $LOG_DIR
mkdir -p $RESULTS_DIR
mkdir -p $STATS_DIR

# Euler Forward

export EXPERIMENT_NAME="euler-forward"
export MOS_SCRIPT="$path/euler-forward.mos"
export SIMULATION_ARGS=""

"$path/compile_simulate_all.sh"

# IDA

export EXPERIMENT_NAME="ida"
export MOS_SCRIPT="$path/ida.mos"
export SIMULATION_ARGS="-noEquidistantTimeGrid"

"$path/compile_simulate_all.sh"

rm -rf $BUILD_DIR
