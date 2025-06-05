cmake_minimum_required(VERSION 3.25.1)

project(
  c-website
  DESCRIPTION "Recreational C website"
  LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(HttpServer REQUIRED)

add_subdirectory(lib)
add_subdirectory(bin)
