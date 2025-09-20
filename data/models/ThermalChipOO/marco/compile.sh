#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

# Instantiate the model
OMC_TIME_FILE=$STATS_DIR/omc-instantiation-time-$EXPERIMENT.txt
OMC_FLAGS="--baseModelica -d=nonfScalarize,arrayConnect,combineSubscripts,evaluateAllParameters,vectorizeBindings"
/usr/bin/time -p -a -o $OMC_TIME_FILE omc $BUILD_DIR/ThermalChipOO.mo -i=ThermalChipOO.Models.ThermalChipSimpleBoundary $OMC_FLAGS 1> $BUILD_DIR/ThermalChipOO-flat.mo
sed -i '1,2d;$d' $BUILD_DIR/ThermalChipOO-flat.mo

# bmodelica dialect size
marco \
  $BUILD_DIR/ThermalChipOO-flat.mo \
  --omc-bypass -O2 \
  --model=ThermalChipSimpleBoundary \
  -o $BUILD_DIR/simulation.mlir \
  --variable-filter="Tct1;Tct2;Tct3;Tct4;Tcb1;Tcb2;Tcb3;Tcb4" \
  -Xmarco -no-multithreading \
  -Xmarco -no-equations-runtime-scheduling \
  -c -emit-mlir \
  $COMPILATION_ARGS || exit 1

BMODELICA_SIZE_FILE=$STATS_DIR/marco-bmodelica-size-$EXPERIMENT.txt
wc -c $BUILD_DIR/simulation.mlir > $BMODELICA_SIZE_FILE

# LLVM-IR size
marco \
  $BUILD_DIR/ThermalChipOO-flat.mo \
  --omc-bypass -O2 \
  --model=ThermalChipSimpleBoundary \
  -o $BUILD_DIR/simulation.ll \
  --variable-filter="Tct1;Tct2;Tct3;Tct4;Tcb1;Tcb2;Tcb3;Tcb4" \
  -Xmarco -no-multithreading \
  -Xmarco -no-equations-runtime-scheduling \
  -c -emit-llvm \
  $COMPILATION_ARGS || exit 1

LLVMIR_SIZE_FILE=$STATS_DIR/marco-llvmir-size-$EXPERIMENT.txt
wc -c $BUILD_DIR/simulation.ll > $LLVMIR_SIZE_FILE

# Binary

# Compile the model
COMPILATION_TIME_FILE=$STATS_DIR/marco-compilation-time-$EXPERIMENT.txt

/usr/bin/time -p -a -o $COMPILATION_TIME_FILE marco \
  $BUILD_DIR/ThermalChipOO-flat.mo \
  --omc-bypass -O2 \
  --model=ThermalChipSimpleBoundary \
  -o $BUILD_DIR/simulation \
  --variable-filter="Tct1;Tct2;Tct3;Tct4;Tcb1;Tcb2;Tcb3;Tcb4" \
  -Xmarco -no-multithreading \
  -Xmarco -no-equations-runtime-scheduling \
  $COMPILATION_ARGS || exit 1

# Get the binary size
BINARY_SIZE_FILE=$STATS_DIR/marco-binary-size-$EXPERIMENT.txt
wc -c $BUILD_DIR/simulation > $BINARY_SIZE_FILE
