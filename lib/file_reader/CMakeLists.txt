add_library(file_reader STATIC file_reader.c file_reader.h)

target_include_directories(file_reader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
