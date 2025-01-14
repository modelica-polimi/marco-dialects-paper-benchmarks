#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

rm -f "$path/marco-benchmarks.sif"
apptainer build "$path/marco-benchmarks.sif" Apptainer.def
