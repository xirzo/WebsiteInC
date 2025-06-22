cmake_minimum_required(VERSION 3.25.1)

project(
  c-website
  DESCRIPTION "Recreational C website"
  LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(HttpServer REQUIRED)

add_executable(${PROJECT_NAME} main.c)

file(COPY ${CMAKE_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE HttpServer::HttpServer)
