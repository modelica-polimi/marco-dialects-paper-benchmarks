#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

rm -rf "$path/marco"
mkdir -p "$path/marco"
git clone https://github.com/marco-compiler/marco.git "$path/marco"
cd "$path/marco"
git checkout 0762e71cc0fb60f60a8949e0e527cedfc9fc5ad5
cd - &> /dev/null
