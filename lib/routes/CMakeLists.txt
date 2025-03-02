add_library(routes STATIC routes.c routes.h)

target_include_directories(routes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
