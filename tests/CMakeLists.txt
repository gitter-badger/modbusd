#
# taka-wang

cmake_minimum_required(VERSION 2.8.9)
project(test_hash C)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(bin_name hash)
set(modbus_include /usr/local/include/modbus)
set(bin_source hash_test.c ../modbusd.h ${modbus_include}/modbus.h)

include_directories(${modbus_include})
add_executable(${bin_name} ${bin_source})
target_link_libraries(${bin_name} LINK_PUBLIC zmq czmq modbus)
