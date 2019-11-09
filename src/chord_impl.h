//
// Created by Inchan Hwang on 2019-11-09.
//

#ifndef CHORD_CHORD_IMPL_H
#define CHORD_CHORD_IMPL_H

#include <utility>
#include <grpcpp/grpcpp.h>
#include "generated/chord.grpc.pb.h"
#include "context.h"

using grpc::ServerContext;
using grpc::Status;
using chord::Chord;
using chord::DumpRequest;
using chord::DumpResponse;

class ChordImpl final : public Chord::Service {
private:
    Context myContext;

public:
    explicit ChordImpl(Context myContext);

    Status DumpNode(ServerContext* context, const DumpRequest* request, DumpResponse* response) override;
};

#endif //CHORD_CHORD_IMPL_H
