cmake_minimum_required(VERSION 3.14)

project(portfolio-optimizer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/home/faraz/Qt/5.15.2/gcc_64/")

find_package(Threads REQUIRED)
find_package(
  QT NAMES Qt6 Qt5
  COMPONENTS Core
  REQUIRED)
find_package(
  Qt${QT_VERSION_MAJOR}
  COMPONENTS Core
  REQUIRED)

include(cmake/googletest.cmake)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(lib)
add_subdirectory(src)

if(UNIX)
  add_subdirectory(test)
endif()
