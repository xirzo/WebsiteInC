add_library(server STATIC server.c server.h)

target_link_libraries(server PUBLIC router)

target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
