#!/bin/bash

g++ -g -march=native -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -c Output.cpp
g++ -g -march=native -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -c MathFunc.cpp
g++ -g -march=native -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -c TensorUtils.cpp
g++ -g -march=native -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -c MatVec.cpp
g++ -g -march=native -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -c ConjugateGradient.cpp
g++ -g -march=native -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -c GMRES.cpp

## Linker
g++ -g -march=native -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -c main_fp32.cpp
g++ -g -march=native -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -o main_fp32.out *.o
objdump -S -d main_fp32.o > main_fp32.asm
rm -rf main_fp32.o

g++ -g -march=native -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -c main_fp64.cpp
g++ -g -march=native -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -o main_fp64.out *.o
objdump -S -d main_fp64.o > main_fp64.asm
rm -rf main_fp64.o

objdump -S -d MathFunc.o > MathFunc.asm
objdump -S -d TensorUtils.o > TensorUtils.asm
objdump -S -d MatVec.o > MatVec.asm
objdump -S -d ConjugateGradient.o > ConjugateGradient.asm
objdump -S -d GMRES.o > GMRES.asm
objdump -S -d main_fp32.out > main_fp32.out.asm
objdump -S -d main_fp64.out > main_fp64.out.asm