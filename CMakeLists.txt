#
# taka-wang

cmake_minimum_required(VERSION 2.8.9)
project(modbusd C)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(bin_name modbusd)
#set(modbus_include /usr/local/include/modbus)
set(bin_source modbusd.c modbusd.h uthash.h )

add_executable(${bin_name} ${bin_source})
target_link_libraries(${bin_name} LINK_PUBLIC zmq czmq modbus)

