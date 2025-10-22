#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
export SIMULATION_ENABLED=1

run_config() {
    "$path/compile_simulate_config.sh" "$@"
    result=$?
    if [ "$result" -eq 1 ]; then
        exit 1
    elif [ "$result" -eq 2 ]; then
        export SIMULATION_ENABLED=0
    elif [ "$result" -ne 0 ]; then
        exit "$result"
    fi
}

run_config 4 4 4
run_config 6 6 4
run_config 8 8 4
run_config 12 12 4
run_config 15 15 5
run_config 18 18 6
run_config 24 24 8
run_config 33 33 11
run_config 39 39 13
run_config 54 54 18
run_config 66 66 22
run_config 84 84 28
run_config 114 114 38
run_config 144 144 48
run_config 183 183 61
