#!/bin/bash

SIMULATION_DIR=$1
SIMULATION_FILE=$2

sim_args=${@:3}

cd $SIMULATION_DIR
./$SIMULATION_FILE -r=results.csv -lv=LOG_STATS_V $sim_args
cd -
