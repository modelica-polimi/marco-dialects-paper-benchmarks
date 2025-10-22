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

run_config 3 4 6
run_config 4 5 5
run_config 4 6 6
run_config 4 8 8
run_config 6 10 10
run_config 6 12 12
run_config 6 16 16
run_config 6 24 24
run_config 8 24 24
run_config 8 32 32
run_config 8 48 48
run_config 12 64 64
run_config 16 86 86
run_config 16 128 128
run_config 16 192 192
run_config 24 192 192
