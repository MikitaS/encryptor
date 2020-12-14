cmake_minimum_required(VERSION 3.10)

# Here must be declarated all .cpp .h files in the project 
set(SOURCES main.cpp)
set(HEADERS include/encryptor.h)

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
                set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=.*" "-warnings-as-errors=.*" "-header-filter=.*") 
endif()

project(encryptor)
add_executable(encryptor ${SOURCES})
target_include_directories(encryptor PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_custom_target(
        clang_tidy
        ${DO_CLANG_TIDY}
        ${PROJECT_SOURCE_DIR}/${SOURCES}
        --
        -I${PROJECT_SOURCE_DIR}/include
        )

add_custom_target(
        clang_format
	${DO_CLANG_FORMAT}
	${PROJECT_SOURCE_DIR}/${SOURCES}
	${PROJECT_SOURCE_DIR}/${HEADERS}
)
