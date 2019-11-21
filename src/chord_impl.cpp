#include <utility>

//
// Created by Inchan Hwang on 2019-11-07.
//

#include <utility>
#include <grpcpp/grpcpp.h>
#include "chord_impl.h"
#include "generated/chord.grpc.pb.h"

using grpc::ServerContext;
using grpc::Status;

ChordImpl::ChordImpl(Context myContext): myContext(std::move(myContext)) { }

//Status ChordImpl::DumpNode(ServerContext* context, const DumpRequest* request, DumpResponse* response) {
//    response->mutable_node()->CopyFrom(myContext.dumpProto());
//    return Status::OK;
//}

Status ChordImpl::join(ServerContext *context, const JoinReq *request, JoinResp *response) {
    return Status::OK;
}

Status ChordImpl::getInfo(ServerContext *context, const GetInfoReq *request, GetInfoResp *response) {
    return Status::OK;
}

Status ChordImpl::findPred(ServerContext *context, const FindPredReq *request, FindPredResp *response) {
    return Status::OK;
}

Status ChordImpl::findSucc(ServerContext *context, const FindSuccReq *request, FindSuccResp *response) {
    return Status::OK;
}

Status ChordImpl::closestPredFinger(ServerContext *context, const ClosestPredFingerReq *request,
                                    ClosestPredFingerResp *response) {
    return Status::OK;
}

Status ChordImpl::stabilize(ServerContext *context, const StabilizeReq *request, StabilizeResp *response) {
    return Status::OK;
}

Status ChordImpl::notify(ServerContext *context, const NotifyReq *request, NotifyResp *response) {
    return Status::OK;
}

Status ChordImpl::fixFingers(ServerContext *context, const FixFingersReq *request, FixFingersResp *response) {
    return Status::OK;
}

