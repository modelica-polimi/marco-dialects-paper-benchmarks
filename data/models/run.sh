#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
date=$(date +%s)

# Set up environment variables
export TOOL_ROOT=/output/tool
export BUILD_ROOT=/output/build
export LOG_ROOT=/output/logs
export RESULTS_ROOT=/output/results
export STATS_ROOT=/output/stats

rm -rf $TOOL_ROOT
rm -rf $BUILD_ROOT
rm -rf $LOG_ROOT
rm -rf $RESULTS_ROOT
rm -rf $STATS_ROOT

# Run the benchmarks
"$path/ThermalChipOO/run.sh"
"$path/MethanolHeatExchangers/run.sh"

# Clean up
rm -rf $TOOL_ROOT
rm -rf $BUILD_ROOT

# Collect the results
tar -czvf /output/run-${date}.tar.gz $LOG_ROOT $RESULTS_ROOT $STATS_ROOT &> /dev/null
