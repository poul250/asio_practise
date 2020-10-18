cmake_minimum_required(VERSION 3.0)

add_executable(echo_server main.cpp)
set_target_properties(echo_server PROPERTIES CXX_STANDARD 20)

target_link_libraries(echo_server INTERFACE asio)
