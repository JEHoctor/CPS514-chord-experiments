//
// Created by Inchan Hwang on 2019-11-07.
//

#include <grpcpp/grpcpp.h>
#include "generated/chord.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using chord::Chord;
using chord::DumpRequest;
using chord::DumpResponse;

class ChordImpl final : public Chord::Service {
    Status DumpNode(ServerContext* context, const DumpRequest* request, DumpResponse* response) override {
//        response->set_allocated_node();
        return Status::OK;
    }
};