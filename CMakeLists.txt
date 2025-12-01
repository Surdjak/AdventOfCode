cmake_minimum_required(VERSION 3.15)

project(AdventOfCode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Common warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif (MSVC)
    add_compile_options(/W4)
endif()

# Utils library (compiled from utils/*.cpp) and headers
file(GLOB UTILS_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/utils/*.cpp)
add_library(aoc_utils STATIC ${UTILS_SOURCES})
target_include_directories(aoc_utils PUBLIC ${CMAKE_SOURCE_DIR}/utils)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Discover each day directory (day1, day2, ...)
file(GLOB DAY_DIRS LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/day*)

foreach(DAY_DIR ${DAY_DIRS})
    if (IS_DIRECTORY ${DAY_DIR} AND EXISTS ${DAY_DIR}/src)
        file(GLOB_RECURSE DAY_SOURCES CONFIGURE_DEPENDS ${DAY_DIR}/src/*.cpp)
        if (DAY_SOURCES)
            get_filename_component(DAY_NAME ${DAY_DIR} NAME)
            add_executable(${DAY_NAME} ${DAY_SOURCES})
            # Allow includes like "utils/file_utils.h" by adding project root
            target_include_directories(${DAY_NAME} PRIVATE ${DAY_DIR}/src ${CMAKE_SOURCE_DIR})
            target_link_libraries(${DAY_NAME} PRIVATE aoc_utils)
            set_target_properties(${DAY_NAME} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            )
        endif()
    endif()
endforeach()
