cmake_minimum_required(VERSION 3.9.6)

project(ExploadEntranceTest CXX)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Select default build type: 'Release'")
  set(CMAKE_BUILD_TYPE "Release")
endif()

# Подключаем поддержку потоков, т.к. она нужна для GTest
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(example)
