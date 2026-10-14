#!/bin/bash

# Create a build directory[cite: 7]
mkdir -p build

# Define the compiler and optimization flags for clang[cite: 7]
CXX="clang++"
CXXFLAGS="-O3 -g --target=riscv64-unknown-elf -march=rv64gcv1p0 -mabi=lp64d -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti"

# Define the RISC-V cross-toolchain prefix[cite: 7]
TOOLCHAIN_PREFIX="riscv64-elf-"
LD="${TOOLCHAIN_PREFIX}g++"
# Use -nostdlib for bare-metal linking since standard libraries (libc/libgloss) aren't present[cite: 7]
LDFLAGS="-nostdlib"
OBJDUMP="${TOOLCHAIN_PREFIX}objdump"

echo "Compiling modules to assembly..."[cite: 7]
$CXX $CXXFLAGS -S Output.cpp -o build/Output.s
$CXX $CXXFLAGS -S MathFunc.cpp -o build/MathFunc.s
$CXX $CXXFLAGS -S TensorUtils.cpp -o build/TensorUtils.s
$CXX $CXXFLAGS -S MatVec.cpp -o build/MatVec.s
$CXX $CXXFLAGS -S ConjugateGradient.cpp -o build/ConjugateGradient.s
$CXX $CXXFLAGS -S GMRES.cpp -o build/GMRES.s
$CXX $CXXFLAGS -S main_fp32.cpp -o build/main_fp32.s
$CXX $CXXFLAGS -S main_fp64.cpp -o build/main_fp64.s

echo "Compiling core object files..."[cite: 7]
$CXX $CXXFLAGS -c Output.cpp -o build/Output.o
$CXX $CXXFLAGS -c MathFunc.cpp -o build/MathFunc.o
$CXX $CXXFLAGS -c TensorUtils.cpp -o build/TensorUtils.o
$CXX $CXXFLAGS -c MatVec.cpp -o build/MatVec.o
$CXX $CXXFLAGS -c ConjugateGradient.cpp -o build/ConjugateGradient.o
$CXX $CXXFLAGS -c GMRES.cpp -o build/GMRES.o

# Helper array of core utility objects[cite: 7]
CORE_OBJS=("build/Output.o" "build/MathFunc.o" "build/TensorUtils.o" "build/MatVec.o" "build/ConjugateGradient.o" "build/GMRES.o")

echo "Building FP32 executable and assembly dump..."[cite: 7]
$CXX $CXXFLAGS -c main_fp32.cpp -o build/main_fp32.o
$LD $LDFLAGS -o build/main_fp32.out build/main_fp32.o "${CORE_OBJS[@]}"
$OBJDUMP -S -d build/main_fp32.o > build/main_fp32.asm

echo "Building FP64 executable and assembly dump..."[cite: 7]
$CXX $CXXFLAGS -c main_fp64.cpp -o build/main_fp64.o
$LD $LDFLAGS -o build/main_fp64.out build/main_fp64.o "${CORE_OBJS[@]}"
$OBJDUMP -S -d build/main_fp64.o > build/main_fp64.asm

echo "Generating module disassembly dumps..."[cite: 7]
$OBJDUMP -S -d build/MathFunc.o > build/MathFunc.asm
$OBJDUMP -S -d build/TensorUtils.o > build/TensorUtils.asm
$OBJDUMP -S -d build/MatVec.o > build/MatVec.asm
$OBJDUMP -S -d build/ConjugateGradient.o > build/ConjugateGradient.asm
$OBJDUMP -S -d build/GMRES.o > build/GMRES.asm

echo "Build complete!"[cite: 7]