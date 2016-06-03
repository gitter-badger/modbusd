#
# taka-wang

cmake_minimum_required(VERSION 2.8.9)
project(test C)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(bin_name test)
set(modbus_include /usr/local/include/modbus)
set(bin_source test.c ../modbusd.c ../json/cJSON.c)

# cflag
set(EXT_C_FLAGS "-W -Wall -Os -std=c99 -DHASH_DEBUG=1")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXT_C_FLAGS}")

include_directories(${modbus_include})
#add_definitions(-std=c99)
add_executable(${bin_name} ${bin_source})
target_link_libraries(${bin_name} LINK_PUBLIC zmq czmq modbus m)
install(TARGETS ${bin_name} RUNTIME DESTINATION /usr/bin/)