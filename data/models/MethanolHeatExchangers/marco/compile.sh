#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

# Instantiate the model
OMC_TIME_FILE=$STATS_DIR/omc-instantiation-time-$EXPERIMENT.txt
OMC_FLAGS="--baseModelica -d=nonfScalarize,arrayConnect,combineSubscripts,evaluateAllParameters,vectorizeBindings"
/usr/bin/time -p -a -o $OMC_TIME_FILE omc $BUILD_DIR/MethanolHeatExchangersDAE.mo -i=MethanolHeatExchangersDAE.Models.MethanolHeatExchangers $OMC_FLAGS 1> $BUILD_DIR/MethanolHeatExchangersDAE-flat.mo
sed -i '1,2d;$d' $BUILD_DIR/MethanolHeatExchangersDAE-flat.mo

# bmodelica dialect size
marco \
  $BUILD_DIR/MethanolHeatExchangersDAE-flat.mo \
  --omc-bypass -O2 \
  --model=MethanolHeatExchangers \
  -o $BUILD_DIR/simulation.mlir \
  --variable-filter="T_m" \
  -Xmarco -no-multithreading \
  -Xmarco -no-equations-runtime-scheduling \
  -c -emit-mlir \
  $COMPILATION_ARGS || exit 1

BMODELICA_SIZE_FILE=$STATS_DIR/marco-bmodelica-size-$EXPERIMENT.txt
wc -c $BUILD_DIR/simulation.mlir > $BMODELICA_SIZE_FILE

# LLVM-IR size
marco \
  $BUILD_DIR/MethanolHeatExchangersDAE-flat.mo \
  --omc-bypass -O2 \
  --model=MethanolHeatExchangers \
  -o $BUILD_DIR/simulation.ll \
  --variable-filter="T_m" \
  -Xmarco -no-multithreading \
  -Xmarco -no-equations-runtime-scheduling \
  -c -emit-llvm \
  $COMPILATION_ARGS || exit 1

LLVMIR_SIZE_FILE=$STATS_DIR/marco-llvmir-size-$EXPERIMENT.txt
wc -c $BUILD_DIR/simulation.ll > $LLVMIR_SIZE_FILE

# Binary
COMPILATION_TIME_FILE=$STATS_DIR/marco-compilation-time-$EXPERIMENT.txt

/usr/bin/time -p -a -o $COMPILATION_TIME_FILE marco \
  $BUILD_DIR/MethanolHeatExchangersDAE-flat.mo \
  --omc-bypass -O2 \
  --model=MethanolHeatExchangers \
  -o $BUILD_DIR/simulation \
  --variable-filter="T_m" \
  -Xmarco -no-multithreading \
  -Xmarco -no-equations-runtime-scheduling \
  $COMPILATION_ARGS || exit 1

# Get the binary size
BINARY_SIZE_FILE=$STATS_DIR/marco-binary-size-$EXPERIMENT.txt
wc -c $BUILD_DIR/simulation > $BINARY_SIZE_FILE
