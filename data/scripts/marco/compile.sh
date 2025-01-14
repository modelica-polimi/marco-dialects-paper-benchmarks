#!/bin/bash

path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

nx=$1
ny=$2
nz=$3
solver=$4

NUM_RUNS=10
OMC_FLAGS="--baseModelica -d=nonfScalarize,arrayConnect,combineSubscripts,evaluateAllParameters,vectorizeBindings"

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

echo "---------------------------------------------"
echo "Compile tool for time statistics"
g++ "$path/time_stat.cpp" -o $BUILD_DIR/time_stat

echo "---------------------------------------------"
echo "Compile tool for size statistics"
g++ "$path/size_stat.cpp" -o $BUILD_DIR/size_stat

echo "---------------------------------------------"
echo "Generate Base Modelica"
OMC_TIMES_FILE=$BUILD_DIR/omc-times_$nx-$ny_$nz-$solver.txt

for ((i = 0; i <= $NUM_RUNS; i++))
do
	/usr/bin/time -p -a -o $OMC_TIMES_FILE omc $SRC_DIR/ThermalChipOO-$nx-$ny-$nz.mo -i=ThermalChipOO.Models.ThermalChipSimpleBoundary $OMC_FLAGS &> /dev/null
done

omc $SRC_DIR/ThermalChipOO-$nx-$ny-$nz.mo -i=ThermalChipOO.Models.ThermalChipSimpleBoundary $OMC_FLAGS 1> $BUILD_DIR/ThermalChipOO-flat-$nx-$ny-$nz.mo

echo "---------------------------------------------"
echo "Computing Base Modelica generation time"
echo "Base Modelica generation time:"
OMC_TIME_FILE=$LOG_DIR/omc-time_$nx-$ny-$nz-$solver.txt
$BUILD_DIR/time_stat $OMC_TIMES_FILE > $OMC_TIME_FILE
cat $OMC_TIME_FILE

echo "---------------------------------------------"
echo "Fix Base Modelica"

cp $BUILD_DIR/ThermalChipOO-flat-$nx-$ny-$nz.mo $BUILD_DIR/ThermalChipOO-flat-fixed-$nx-$ny-$nz.mo
sed -i '1,2d;$d' $BUILD_DIR/ThermalChipOO-flat-fixed-$nx-$ny-$nz.mo

echo "---------------------------------------------"
echo "Generating code sizes"
MODELICA_DIALECT_SIZES_FILE=$LOG_DIR/bmodelica-sizes_$nx-$ny-$nz-$solver.txt
LLVMIR_SIZES_FILE=$LOG_DIR/llvmir-sizes_$nx-$ny-$nz-$solver.txt

for ((i = 0; i <= $NUM_RUNS; i++))
do
marco \
  -mc1 \
  -no-multithreading \
  -Xmarco -no-equations-runtime-scheduling \
  $BUILD_DIR/ThermalChipOO-flat-fixed-$nx-$ny-$nz.mo \
  --omc-bypass -O2 \
  --model=ThermalChipSimpleBoundary \
  --solver=$solver \
  -emit-mlir \
  --variables-filter="Tct1;Tct2;Tct3;Tct4;Tcb1;Tcb2;Tcb3;Tcb4" \
  -o $BUILD_DIR/model.mlir

wc -c $BUILD_DIR/model.mlir >> $MODELICA_DIALECT_SIZES_FILE

marco \
  $BUILD_DIR/ThermalChipOO-flat-fixed-$nx-$ny-$nz.mo \
  -Xmarco -no-multithreading \
  -Xmarco -no-equations-runtime-scheduling \
  --omc-bypass -O2 \
  --model=ThermalChipSimpleBoundary \
  --solver=$solver \
  -c -emit-llvm \
  --variables-filter="Tct1;Tct2;Tct3;Tct4;Tcb1;Tcb2;Tcb3;Tcb4" \
  -o $BUILD_DIR/model.bc

llvm-dis $BUILD_DIR/model.bc -o $BUILD_DIR/model.ll
wc -c $BUILD_DIR/model.ll >> $LLVMIR_SIZES_FILE
done

echo "---------------------------------------------"
echo "Computing code sizes"
echo "------"
echo "Modelica dialect size:"
MODELICA_DIALECT_SIZE_FILE=$LOG_DIR/bmodelica-size_$nx-$ny-$nz-$solver.txt
$BUILD_DIR/size_stat $MODELICA_DIALECT_SIZES_FILE > $MODELICA_DIALECT_SIZE_FILE
cat $MODELICA_DIALECT_SIZE_FILE

echo "------"
echo "LLVM-IR size:"
LLVMIR_SIZE_FILE=$LOG_DIR/llvmir-size_$nx-$ny-$nz-$solver.txt
$BUILD_DIR/size_stat $LLVMIR_SIZES_FILE > $LLVMIR_SIZE_FILE
cat $LLVMIR_SIZE_FILE

echo "---------------------------------------------"
echo "Build simulation"
MARCO_COMPILE_TIMES_FILE=$LOG_DIR/marco-compile-times_$nx-$ny-$nz-$solver.txt

for ((i = 0; i <= $NUM_RUNS; i++))
do
/usr/bin/time -p -a -o $MARCO_COMPILE_TIMES_FILE marco \
  $BUILD_DIR/ThermalChipOO-flat-fixed-$nx-$ny-$nz.mo \
  -Xmarco -no-multithreading \
  -Xmarco -no-equations-runtime-scheduling \
  --omc-bypass -O2 \
  --model=ThermalChipSimpleBoundary \
  --solver=$solver \
  -o $BUILD_DIR/simulation-$nx-$ny-$nz-$solver \
  --variables-filter="Tct1;Tct2;Tct3;Tct4;Tcb1;Tcb2;Tcb3;Tcb4"
done

echo "---------------------------------------------"
echo "MARCO build time:"
MARCO_COMPILE_TIME_FILE=$LOG_DIR/marco-compile-time_$nx-$ny-$nz-$solver.txt
$BUILD_DIR/time_stat $MARCO_COMPILE_TIMES_FILE > $MARCO_COMPILE_TIME_FILE
cat $MARCO_COMPILE_TIME_FILE

echo "---------------------------------------------"
echo "Binary size:"
MARCO_BINARY_SIZE_FILE=$LOG_DIR/marco-binary-size_$nx-$ny-$nz-$solver.txt
wc -c $BUILD_DIR/simulation-$nx-$ny-$nz-$solver > $MARCO_BINARY_SIZE_FILE
cat $MARCO_BINARY_SIZE_FILE
