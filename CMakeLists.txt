cmake_minimum_required(VERSION 3.13)
project(chord)

set(CMAKE_CXX_STANDARD 11)

# GRPC and Protocol Buffers libraries location
list(APPEND CMAKE_PREFIX_PATH "/opt/grpc" "/opt/protobuf")
list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/openssl")

# Cmake find modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# find protobuf and grpc
find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

# find openssl
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OPENSSL REQUIRED)

# source/test
include_directories(src)
add_subdirectory(src)
add_subdirectory(tst)
add_subdirectory(lib/googletest)
