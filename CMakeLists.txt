cmake_minimum_required(VERSION 3.14)

project(portfolio-optimizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(cmake/googletest.cmake)
include(cmake/nlohmann-json.cmake)
include(cmake/clang-tidy.cmake)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)
