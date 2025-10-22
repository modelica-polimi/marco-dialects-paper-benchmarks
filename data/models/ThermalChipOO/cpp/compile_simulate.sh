#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

echo "-----------------------------------"
echo "Experiment: $EXPERIMENT"
timeout $COMPILATION_TIMEOUT "$path/compile.sh" || exit 1

if [ "$SIMULATION_ENABLED" = "1" ]; then
    timeout $SIMULATION_TIMEOUT "$path/simulate.sh" || exit 2
fi
echo "-----------------------------------"
