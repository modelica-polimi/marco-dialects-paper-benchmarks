#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

mkdir -p "$path/csv_exporter/build"
mkdir -p "$path/csv_exporter/install"

cmake -S "$path/csv_exporter" -B "$path/csv_exporter/build" -DCMAKE_INSTALL_PREFIX="$path/csv_exporter/install"
cmake --build "$path/csv_exporter/build" --target install
