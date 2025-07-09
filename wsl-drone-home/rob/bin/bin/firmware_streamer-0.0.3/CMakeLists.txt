cmake_minimum_required(VERSION 3.8)
project(firmware_streamer)
# find dependencies
find_package(Boost  REQUIRED COMPONENTS system) # serial
#
find_library(MOSQUITTOPP_LIB  mosquittopp ) # mqtt c++ mosquitto
# set src files
set(src_files src/data_format/data_format.cpp
              src/logger/logger.cpp
              src/protocol/mqtt_client.cpp
              src/protocol/protocol_base.cpp
              src/protocol/serial.cpp
              src/protocol/udp_client.cpp
              src/connections/connections.cpp)
#
add_executable(run_connections  demo/run_connections.cpp ${src_files})
# link include
target_include_directories(run_connections PRIVATE ${CMAKE_SOURCE_DIR}/include)
# Link libraries
target_link_libraries(run_connections ${MOSQUITTOPP_LIB}
                                      Boost::system
                                      pthread)
