cmake_minimum_required(VERSION 3.0)

add_executable(client client.cpp)
set_target_properties(client PROPERTIES CXX_STANDARD 20)
target_link_libraries(client INTERFACE asio)

add_executable(server server.cpp)
set_target_properties(server PROPERTIES CXX_STANDARD 20)
target_link_libraries(server INTERFACE asio)
