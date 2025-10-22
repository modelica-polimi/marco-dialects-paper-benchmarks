#!/bin/bash

TIME_FILE=$STATS_DIR/cpp-simulation-time-$EXPERIMENT.txt
/usr/bin/time -p -o $TIME_FILE $BUILD_DIR/simulation $RESULTS_DIR/$EXPERIMENT.csv nx=$nx ny=$ny nz=$nz $SIMULATION_ARGS || exit 1
