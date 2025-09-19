#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

TIME_FILE=$STATS_DIR/omc-simulation-time-$EXPERIMENT.txt
/usr/bin/time -p -o $TIME_FILE "$path/simulation_run.sh" $BUILD_DIR MethanolHeatExchangersDAE.Models.MethanolHeatExchangers || exit 1
mv "$BUILD_DIR/results.csv" "$RESULTS_DIR/$EXPERIMENT.csv"
