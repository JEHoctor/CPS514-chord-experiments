//
// Created by Inchan Hwang on 2019-11-09.
//

#include <utility>
#include <string>
#include <grpcpp/grpcpp.h>
#include "context.h"
#include "generated/chord.grpc.pb.h"

using chord::Chord;

void startServer(const std::string& port, Chord::Service* chordService) {
    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:"+port, grpc::InsecureServerCredentials());
    builder.RegisterService(chordService);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << port << std::endl;
    server->Wait();
}
