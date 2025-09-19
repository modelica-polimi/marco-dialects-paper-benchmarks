#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

"$path/docker_upgrade.sh"
"$path/docker_build.sh"
mkdir -p "$path/output"

docker run --rm \
	-u $(id -u):$(id -g) \
	-v "$path/data":/data \
	-v "$path/output":/output \
	-e COMPILATION_TIMEOUT=3600 \
	-e SIMULATION_TIMEOUT=10800 \
	marco-benchmarks \
	bash -c "/data/run.sh"
