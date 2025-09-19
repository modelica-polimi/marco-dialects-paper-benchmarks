#!/bin/bash

TIME_FILE=$STATS_DIR/marco-simulation-time-$EXPERIMENT.txt
/usr/bin/time -p -o $TIME_FILE $BUILD_DIR/simulation $SIMULATION_ARGS 1> $RESULTS_DIR/$EXPERIMENT.csv || exit 1
