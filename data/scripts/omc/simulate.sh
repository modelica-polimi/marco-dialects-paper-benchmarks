#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

nx=$1
ny=$2
nz=$3
solver=$4
sim_args=${@:5}

TIME_FILE=$LOG_DIR/simulation-time_$nx-$ny-$nz-$solver.txt
/usr/bin/time -p -o $TIME_FILE "$path/simulation_run.sh" $BUILD_DIR ThermalChipOO.Models.ThermalChipSimpleBoundary $sim_args || exit 1
mv "$BUILD_DIR/results.csv" "$RESULTS_DIR/results-$nx-$ny-$nz-$solver.csv"
cat $TIME_FILE
