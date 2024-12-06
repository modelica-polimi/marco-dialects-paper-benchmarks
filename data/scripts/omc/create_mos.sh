#!/bin/bash

nx=$1
ny=$2
nz=$3
solver=$4
outfile=$5

echo "loadFile(\"$SRC_DIR/ThermalChipOO-$nx-$ny-$nz.mo\");" > $outfile
cat script_base.mos >> $outfile
