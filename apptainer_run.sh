#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

"$path/apptainer_build.sh"
mkdir -p "$path/output"
mkdir -p "$path/root"

apptainer exec \
	--bind "$path/data":/data \
	--bind "$path/output":/output \
	--bind "$path/root":/root \
	--env COMPILATION_TIMEOUT=3600 \
	--env SIMULATION_TIMEOUT=10800 \
	"$path/marco-benchmarks.sif" \
	bash -c /data/run.sh
