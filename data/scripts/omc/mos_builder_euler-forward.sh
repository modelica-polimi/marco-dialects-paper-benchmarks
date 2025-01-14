#!/bin/bash

nx=$1
ny=$2
nz=$3
outfile=$4
resultsfile=$5

echo "loadFile(\"$SRC_DIR/ThermalChipOO-$nx-$ny-$nz.mo\");" > $outfile
echo "getErrorString();" >> $outfile
echo "buildModel(" >> $outfile
echo "    ThermalChipOO.Models.ThermalChipSimpleBoundary," >> $outfile
echo "    method=\"euler\"," >> $outfile
echo "    stopTime=0.4," >> $outfile
echo "    numberOfIntervals=266667," >> $outfile
echo "    outputFormat=\"csv\"," >> $outfile
echo "    variableFilter=\"Tct1|Tct2|Tct3|Tct4|Tcb1|Tcb2|Tcb3|Tcb4\"," >> $outfile
echo "    cflags=\"-O0\"," >> $outfile
echo "    simflags=\"-lv=LOG_STATS_V -r=\\\"results.csv\\\"\");" >> $outfile
echo "getErrorString();" >> $outfile
