add_library(router STATIC router.c router.h)

target_include_directories(router PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
