#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

nx=$1
ny=$2
nz=$3
solver=$4
sim_args=${@:5}

"$path/compile_simulate.sh" $nx $ny $nz $solver $sim_args 1> $LOG_DIR/log_$nx-$ny-$nz-$solver.out 2> $LOG_DIR/log_$nx-$ny-$nz-$solver.err
