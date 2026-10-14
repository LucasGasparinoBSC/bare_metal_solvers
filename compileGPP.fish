# compileGPP.fish

# Create a build directory
mkdir -p build

# Common compiler flags in Fish (using an array/list)
set -l CXXFLAGS -O3 -g -march=native -fno-math-errno -fno-exceptions -fno-rtti -DARR_SIZE=18 -DMITERS=12

echo "Compiling modules..."
g++ $CXXFLAGS -c Output.cpp -o build/Output.o
g++ $CXXFLAGS -c MathFunc.cpp -o build/MathFunc.o
g++ $CXXFLAGS -c TensorUtils.cpp -o build/TensorUtils.o
g++ $CXXFLAGS -c MatVec.cpp -o build/MatVec.o
g++ $CXXFLAGS -c ConjugateGradient.cpp -o build/ConjugateGradient.o
g++ $CXXFLAGS -c GMRES.cpp -o build/GMRES.o

# Helper list of utility objects
set -l CORE_OBJS build/Output.o build/MathFunc.o build/TensorUtils.o build/MatVec.o build/ConjugateGradient.o build/GMRES.o

echo "Building FP32 executable..."
g++ $CXXFLAGS -c main_fp32.cpp -o build/main_fp32.o
g++ -o main_fp32.out build/main_fp32.o $CORE_OBJS
objdump -S -d build/main_fp32.o > main_fp32.asm

echo "Building FP64 executable..."
g++ $CXXFLAGS -c main_fp64.cpp -o build/main_fp64.o
g++ -o main_fp64.out build/main_fp64.o $CORE_OBJS
objdump -S -d build/main_fp64.o > main_fp64.asm

echo "Generating assembly dumps for modules..."
objdump -S -d build/MathFunc.o > MathFunc.asm
objdump -S -d build/TensorUtils.o > TensorUtils.asm
objdump -S -d build/MatVec.o > MatVec.asm
objdump -S -d build/ConjugateGradient.o > ConjugateGradient.asm
objdump -S -d build/GMRES.o > GMRES.asm

echo "Build complete!"