set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SRCS LIST_DIRECTORIES true *.h *.cpp)

add_executable(${BINARY}
        ${SRCS}
        ${PROTO_SRCS}
        ${GRPC_SRCS}
)
target_link_libraries(${BINARY}
        gRPC::grpc++_reflection
        protobuf::libprotobuf
)
add_library(${BINARY}_lib STATIC
        ${SRCS}
        ${PROTO_SRCS}
        ${GRPC_SRCS}
)
target_link_libraries(${BINARY}_lib
        gRPC::grpc++_reflection
        protobuf::libprotobuf
)
