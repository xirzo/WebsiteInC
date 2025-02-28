# Build command example
# cmake --preset=clang -D CMAKE_BUILD_TYPE=DEBUG ../..

cmake_minimum_required(VERSION 3.31.4)

project(
  sample
  DESCRIPTION "SAMPLE DESCRIPTION"
  LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib)
add_subdirectory(bin)
