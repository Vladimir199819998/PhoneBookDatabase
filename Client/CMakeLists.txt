cmake_minimum_required(VERSION 3.13)
project(Client)

set(CMAKE_CXX_STANDARD 14)

include_directories("include")
add_executable(Client main.cpp include/Client.h src/Client.cpp)