cmake_minimum_required(VERSION 3.0)

project(Practise VERSION 1.2.3.4 LANGUAGES CXX)

add_library(asio INTERFACE)
target_sources(asio INTERFACE ${CMAKE_CURRENT_DIR}/submodules/asio/asio/include)
target_include_directories(asio INTERFACE ${CMAKE_CURRENT_DIR}/submodules/asio/asio/include)

set(CMAKE_CXX_FLAGS "-pthread -fcoroutines")

add_subdirectory(src)
