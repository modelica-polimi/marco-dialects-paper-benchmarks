#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

nx=$1
ny=$2
nz=$3
solver=$4

fail() {
	rm -rf "$BUILD_DIR"
	mkdir -p "$BUILD_DIR"
	exit 1
}

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

"$path/mos_builder_$solver.sh" $nx $ny $nz $BUILD_DIR/script.mos $RESULTS_DIR/results_$nx-$ny-$nz-$solver.csv
/usr/bin/time -p -o $LOG_DIR/omc-time_$nx-$ny-$nz-$solver.txt "$path/run_mos.sh" $BUILD_DIR || fail

echo "Binary size: "
BINARY_SIZE_FILE=$LOG_DIR/omc-binary-size_$nx-$ny-$nz-$solver.txt
wc -c $BUILD_DIR/ThermalChipOO.Models.ThermalChipSimpleBoundary > $BINARY_SIZE_FILE
cat $BINARY_SIZE_FILE

echo "C code size: "
C_SIZE_FILE=$LOG_DIR/omc-c-size_$nx-$ny-$nz-$solver.txt
du -scb $BUILD_DIR/*.h $BUILD_DIR/*.c $BUILD_DIR/*.cpp | tail -n1 > $C_SIZE_FILE
cat $C_SIZE_FILE
