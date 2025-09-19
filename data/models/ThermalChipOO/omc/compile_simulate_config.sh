#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

export nx=$1
export ny=$2
export nz=$3

export EXPERIMENT=${EXPERIMENT_NAME}-$nx-$ny-$nz

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

"$path/compile_simulate.sh" 1> $LOG_DIR/$EXPERIMENT.out 2> $LOG_DIR/$EXPERIMENT.err
