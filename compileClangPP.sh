#!/bin/bash

clang++ -g --target=riscv64-unknown-elf -march=rv64gcv1p0 -mabi=lp64d -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -S MathFunc.cpp -o MathFunc.s
clang++ -g --target=riscv64-unknown-elf -march=rv64gcv1p0 -mabi=lp64d -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -S TensorUtils.cpp -o TensorUtils.s
clang++ -g --target=riscv64-unknown-elf -march=rv64gcv1p0 -mabi=lp64d -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -S MatVec.cpp -o MatVec.s
clang++ -g --target=riscv64-unknown-elf -march=rv64gcv1p0 -mabi=lp64d -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -S ConjugateGradient.cpp -o ConjugateGradient.s
clang++ -g --target=riscv64-unknown-elf -march=rv64gcv1p0 -mabi=lp64d -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -S GMRES.cpp -o GMRES.s
clang++ -g --target=riscv64-unknown-elf -march=rv64gcv1p0 -mabi=lp64d -fno-math-errno -ffreestanding -fno-exceptions -fno-rtti -S main_fp32.cpp -o main_fp32.s