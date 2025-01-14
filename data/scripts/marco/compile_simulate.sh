#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

nx=$1
ny=$2
nz=$3
solver=$4

echo "nx=$nx, ny=$ny, nz=$nz, solver=$solver"

echo "Compiling"
timeout $COMPILE_TIMEOUT "$path/compile.sh" $nx $ny $nz $solver

echo "Simulating"
timeout $SIMULATE_TIMEOUT "$path/simulate.sh" $nx $ny $nz $solver ${@:5}

echo "-----------------------------------"
echo ""
