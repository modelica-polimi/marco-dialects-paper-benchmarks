#!/bin/bash

if [ "$#" -ne 5 ]; then
    echo "Usage: $(basename "$0") src_file dst_file Nu Nh Nv"
fi

src=$1
dst=$2
N=$3
M=$4
P=$5

cp "$src" "$dst"
sed -i "s/\(parameter Integer Nu = \)[0-9]\+/\1$Nu/" "$dst"
sed -i "s/\(parameter Integer Nh = \)[0-9]\+/\1$Nh/" "$dst"
sed -i "s/\(parameter Integer Nv = \)[0-9]\+/\1$Nv/" "$dst"
