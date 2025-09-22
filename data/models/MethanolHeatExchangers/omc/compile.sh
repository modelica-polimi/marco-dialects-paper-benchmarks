#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

# Prepare the MOS script
cp "$MOS_SCRIPT" "$BUILD_DIR/compile.mos"

# Compile the model
COMPILATION_TIME_FILE=$STATS_DIR/omc-compilation-time-$EXPERIMENT.txt
/usr/bin/time -p -o $COMPILATION_TIME_FILE "$path/run_mos.sh" || exit 1

# Get the C code size
C_SIZE_FILE=$STATS_DIR/omc-c-size-$EXPERIMENT.txt
du -scb $BUILD_DIR/*.h $BUILD_DIR/*.c | tail -n1 > $C_SIZE_FILE

# Get the binary size
BINARY_SIZE_FILE=$STATS_DIR/omc-binary-size-$EXPERIMENT.txt
wc -c $BUILD_DIR/MethanolHeatExchangersDAE.Models.MethanolHeatExchangers > $BINARY_SIZE_FILE
