#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

export Nu=$1
export Nh=$2
export Nv=$3

export EXPERIMENT=${EXPERIMENT_NAME}-$Nu-$Nh-$Nv

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

"$path/compile_simulate.sh" 1> $LOG_DIR/$EXPERIMENT.out 2> $LOG_DIR/$EXPERIMENT.err
