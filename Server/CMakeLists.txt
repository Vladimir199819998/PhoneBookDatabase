cmake_minimum_required(VERSION 3.13)
project(Server)

set(CMAKE_CXX_STANDARD 14)
add_executable(Server main.cpp src/Server.cpp ../Client/src/Client.cpp)


# includes
include_directories(include libs/include/cppconn ../Client/include/)

# third-party libs
add_library(mysqlcppconn SHARED IMPORTED)
set_property(TARGET mysqlcppconn PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/libmysqlcppconn.so)

# linkage
target_link_libraries(Server mysqlcppconn)
