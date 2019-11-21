#include <utility>

#include <utility>

//
// Created by Inchan Hwang on 2019-11-21.
//

#include "chord_client.h"

using grpc::Status;
using grpc::ClientContext;

std::shared_ptr<grpc::Channel> makeChannel(Node target) {
    return grpc::CreateChannel(target.getAddr(), grpc::InsecureChannelCredentials());
}

bool ChordClient::getInfo(Node target, chord::NodeInfo* dst) {
    auto channel = makeChannel(std::move(target));
    auto stub = chord::Chord::NewStub(channel);
    ClientContext clientCtx;

    chord::GetInfoReq req;
    chord::GetInfoResp resp;

    Status status = stub->getInfo(&clientCtx, req, &resp);

    if(status.ok()) {
        dst->CopyFrom(resp.info());
        return true;
    }

    return false;
}

bool ChordClient::findSucc(Node target, uint32_t key, Node* dst) {
    auto channel = makeChannel(std::move(target));
    auto stub = chord::Chord::NewStub(channel);
    ClientContext clientCtx;

    chord::FindSuccReq req;
    chord::FindSuccResp resp;
    req.set_key(key);

    Status status = stub->findSucc(&clientCtx, req, &resp);

    if(status.ok()) {
        dst->set(resp.mutable_succ()->addr());
        return true;
    }

    return false;
}

bool ChordClient::findPred(Node target, uint32_t key, Node* dst) {
    auto channel = makeChannel(std::move(target));
    auto stub = chord::Chord::NewStub(channel);
    ClientContext clientCtx;

    chord::FindPredReq req;
    chord::FindPredResp resp;
    req.set_key(key);

    Status status = stub->findPred(&clientCtx, req, &resp);

    if(status.ok()) {
        dst->set(resp.mutable_pred()->addr());
        return true;
    }

    return false;
}

void ChordClient::getClosestFinger(Node target, uint32_t key, Node* dst) {
    auto channel = makeChannel(target);
    auto stub = chord::Chord::NewStub(channel);
    ClientContext clientCtx;

    chord::ClosestPredFingerReq req;
    chord::ClosestPredFingerResp resp;
    req.set_key(key);

    Status status = stub->closestPredFinger(&clientCtx, req, &resp);

    if(status.ok()) {
        dst->set(resp.mutable_pred_finger()->addr());
    } else {
        dst->set(target.getAddr());
    }
}

void ChordClient::notify(Node target, Node potentialPred) {
    auto channel = makeChannel(std::move(target));
    auto stub = chord::Chord::NewStub(channel);
    ClientContext clientCtx;

    chord::NotifyReq req;
    req.set_allocated_potential_pred(potentialPred.genProto());
    chord::NotifyResp resp;

    stub->notify(&clientCtx, req, &resp);
}
