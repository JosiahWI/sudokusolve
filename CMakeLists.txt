cmake_minimum_required(VERSION 3.16)

project(sudokusolver
        VERSION 1.0
        DESCRIPTION "A solver for easy sudoku problems."
        LANGUAGES CXX
)

if(CMAKE_BUILD_TYPE STREQUAL "")
    message(WARNING "Consider setting a build type!")
else()
    message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode.")
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# sudokusolver target
add_executable(sudokusolver main.cpp sudoku_solver.cpp)
target_compile_features(sudokusolver PRIVATE cxx_std_11)

target_include_directories(sudokusolver PRIVATE ${TARGET_SOURCE_DIR})

target_compile_options(sudokusolver
    PRIVATE
        -pedantic-errors
        -Werror
        -Wall
        -Weffc++
        -Wextra
        -Wsign-conversion
)

if(CMAKE_CROSSCOMPILING)
    # Link static std libs if cross compiling.
    target_link_libraries(sudokusolver
        PRIVATE
            -static-libgcc
            -static-libstdc++
    )
    # Build for skylake architecture if cross compiling.
    target_compile_options(sudokusolver
        PRIVATE
            -march=skylake
    )
else()
    # If not cross compiling.
    target_compile_options(sudokusolver
        PRIVATE
            -march=native
    )
endif()
