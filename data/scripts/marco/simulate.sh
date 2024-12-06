#!/bin/bash

nx=$1
ny=$2
nz=$3
solver=$4
sim_args=${@:5}

TIME_FILE=$LOG_DIR/simulation-time_$nx-$ny-$nz-$solver.txt
/usr/bin/time -p -o $TIME_FILE $BUILD_DIR/simulation-$nx-$ny-$nz-$solver $sim_args > $RESULTS_DIR/results-$nx-$ny-$nz-$solver.csv
cat $TIME_FILE
