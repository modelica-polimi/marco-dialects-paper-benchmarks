#!/bin/bash

if [ "$#" -ne 5 ]; then
    echo "Usage: $(basename "$0") src_file dst_file N M P"
fi

src=$1
dst=$2
N=$3
M=$4
P=$5

cp "$src" "$dst"
sed -i "s/\(parameter Integer N = \)[0-9]\+/\1$N/" "$dst"
sed -i "s/\(parameter Integer M = \)[0-9]\+/\1$M/" "$dst"
sed -i "s/\(parameter Integer P = \)[0-9]\+/\1$P/" "$dst"
