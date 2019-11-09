//
// Created by Inchan Hwang on 2019-11-07.
//

#include <utility>
#include <grpcpp/grpcpp.h>
#include "chord_impl.h"
#include "generated/chord.grpc.pb.h"
#include "context.h"

using grpc::ServerContext;
using grpc::Status;
using chord::DumpRequest;
using chord::DumpResponse;

ChordImpl::ChordImpl(Context myContext): myContext(std::move(myContext)) { }

Status ChordImpl::DumpNode(ServerContext* context, const DumpRequest* request, DumpResponse* response) {
    response->mutable_node()->CopyFrom(myContext.dumpProto());
    return Status::OK;
}
