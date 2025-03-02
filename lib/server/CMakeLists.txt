add_library(server STATIC server.c server.h)

target_link_libraries(
  server
  PRIVATE HttpParser::http_parser file_reader
  PUBLIC routes)

target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
