cmake_minimum_required(VERSION 3.13)

# Here must be declarated all .cpp .h files in the project 
set(SOURCES 
        ${CMAKE_SOURCE_DIR}/main.cpp
        ${CMAKE_SOURCE_DIR}/libs/cpuid.cpp
        ${CMAKE_SOURCE_DIR}/encryptor/encryptor.cpp)

set(HEADERS 
	${CMAKE_SOURCE_DIR}/include/encryptor.h
	${CMAKE_SOURCE_DIR}/include/cpuid.h)

find_program( CLANG_FORMAT_EXE NAMES "clang-format" DOC "Path to clang-format executable" )
        if(NOT CLANG_FORMAT_EXE) 
		message(FATAL_ERROR "clang-format not found. Please install the toolchain or add it to the PATH") 
        else() 
		message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")
                set(DO_CLANG_FORMAT "${CLANG_FORMAT_EXE}" "-i" "-style=file") 
endif()

find_program( CLANG_TIDY_EXE NAMES "clang-tidy" DOC "Path to clang-tidy executable" )
        if(NOT CLANG_TIDY_EXE) 
		message(FATAL_ERROR "clang-tidy not found. Please install the toolchain or add it to the PATH") 
        else() 
		message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
                set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=file" "-warnings-as-errors=file" "-header-filter=.*") 
endif()

project(encryptor)
add_executable(encryptor ${SOURCES})
target_include_directories(encryptor PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(encryptor PUBLIC ssl crypto)

add_custom_target(
        clang_tidy
        ${DO_CLANG_TIDY}
        ${SOURCES}
        --
        -I${CMAKE_SOURCE_DIR}/include
        -lcrypto
        )

add_custom_target(
        clang_format
	${DO_CLANG_FORMAT}
	${SOURCES}
	${HEADERS}
)

add_dependencies(encryptor clang_tidy)
