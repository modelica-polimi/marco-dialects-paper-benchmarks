#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

docker run --rm -ti -v "$path/data":/data -v "$path/marco":/tmp/marco-src -v "$path/output":/output marco-benchmarks
