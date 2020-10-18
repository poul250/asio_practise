cmake_minimum_required(VERSION 3.0)

add_executable(basic main.cpp)
set_target_properties(basic PROPERTIES CXX_STANDARD 20)
target_link_libraries(basic INTERFACE asio)
