#
# taka-wang
#

cmake_minimum_required(VERSION 2.8.9)
project(modbusd C)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(bin_name modbusd)
set(modbus_include /usr/local/include/modbus)
set(bin_source json/cJSON.c json.c main.c modbusd.c)

# cflag
set(EXT_C_FLAGS "-W -Wall -Wno-format-security -O3 -std=c99")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXT_C_FLAGS}")

# set install prefix
SET(CMAKE_INSTALL_PREFIX /)

include_directories(${modbus_include})
add_executable(${bin_name} ${bin_source} modbusd.json)
target_link_libraries(${bin_name} LINK_PUBLIC zmq czmq modbus m)

# install binary
install(TARGETS ${bin_name} DESTINATION /usr/bin COMPONENT binaries)
# install config
install(FILES modbusd.json DESTINATION /etc/${bin_name} COMPONENT config)
