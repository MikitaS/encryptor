cmake_minimum_required(VERSION 3.10)

set(SOURCES main.cpp)

project(elgamal)
add_executable(elgamal ${SOURCES})
target_include_directories(elgamal PUBLIC "${PROJECT_SOURCE_DIR}/include")

set(HEADERS include/encryptor.h)
add_custom_target(
        clangformat
        COMMAND /usr/bin/clang-format
        -style=LLVM
        -i
	${PROJECT_SOURCE_DIR}/${SOURCES}
	${PROJECT_SOURCE_DIR}/${HEADERS}
)
