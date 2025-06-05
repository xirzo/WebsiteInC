add_executable(${PROJECT_NAME} main.c)

file(COPY ${CMAKE_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_BINARY_DIR}/bin)

target_link_libraries(${PROJECT_NAME} PRIVATE HttpServer::HttpServer)
