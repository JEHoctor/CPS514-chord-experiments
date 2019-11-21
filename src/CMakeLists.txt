set(BINARY ${CMAKE_PROJECT_NAME})

# Compile proto files
file(GLOB PROTOS ${CMAKE_CURRENT_LIST_DIR}/../protodefs/*.proto)
set(PROTO_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_SRC_DIR} ${PROTOS})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_SRC_DIR} ${PROTOS})

file(GLOB_RECURSE SRCS LIST_DIRECTORIES true *.h *.cpp)

add_executable(${BINARY}
        ${SRCS}
        ${PROTO_SRCS}
        ${GRPC_SRCS} chord_client.cpp chord_client.h)
target_link_libraries(${BINARY}
        gRPC::grpc++_reflection
        protobuf::libprotobuf
        OpenSSL::Crypto
)
add_library(${BINARY}_lib STATIC
        ${SRCS}
        ${PROTO_SRCS}
        ${GRPC_SRCS})
target_link_libraries(${BINARY}_lib
        gRPC::grpc++_reflection
        protobuf::libprotobuf
        OpenSSL::Crypto
)
