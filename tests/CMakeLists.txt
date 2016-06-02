#
# taka-wang

cmake_minimum_required(VERSION 2.8.9)
project(test_hash C)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(bin_name hash)
set(modbus_include /usr/local/include/modbus)
set(bin_source hash_test.c ../modbusd.h ${modbus_include}/modbus.h)

# cflag
set(EXT_C_FLAGS "-O3 -std=c99 -DHASH_DEBUG=1 -DHASH_BLOOM=32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXT_C_FLAGS}")

include_directories(${modbus_include})
#add_definitions(-std=c99)
add_executable(${bin_name} ${bin_source})
target_link_libraries(${bin_name} LINK_PUBLIC zmq czmq modbus)
