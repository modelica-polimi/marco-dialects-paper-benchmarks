#!/bin/bash

echo "Running benchmarks for ThermalChipOO"

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
TOOL_DIR=$TOOL_ROOT/ThermalChipOO

# Build the csv_exporter tool
mkdir -p $TOOL_DIR/build/csv_exporter
mkdir -p $TOOL_DIR/install/csv_exporter

cmake -S "$path/csv_exporter" -B $TOOL_DIR/build/csv_exporter -DCMAKE_INSTALL_PREFIX=$TOOL_DIR/install/csv_exporter
cmake --build $TOOL_DIR/build/csv_exporter --target install

# Set environemnt variables for the benchmarks
export MODEL_BUILD_ROOT=$BUILD_ROOT/ThermalChipOO
export MODEL_LOG_ROOT=$LOG_ROOT/ThermalChipOO
export MODEL_RESULTS_ROOT=$RESULTS_ROOT/ThermalChipOO
export MODEL_STATS_ROOT=$STATS_ROOT/ThermalChipOO

# Run the benchmarks
$path/marco/run.sh
$path/omc/run.sh

# Export the statistics
echo "Exporting statistics for ThermalChipOO"
$TOOL_DIR/install/csv_exporter/bin/csv_exporter $MODEL_STATS_ROOT euler-forward > $STATS_ROOT/ThermalChipOO-euler-forward.csv
$TOOL_DIR/install/csv_exporter/bin/csv_exporter $MODEL_STATS_ROOT ida > $STATS_ROOT/ThermalChipOO-ida.csv

# Clean up
rm -rf $TOOL_DIR
