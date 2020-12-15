# Encryptor

# Pre-requirements: 
clang++

clang-format

clang-tidy

make

libssl-dev

cmake

# Build:
cd <SOURCE_DIR> 

mkdir build && cd build

cmake ..

make -j8 encryptor

# Before commiting:
make -j8 clang_format
