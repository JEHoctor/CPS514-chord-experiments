//
// Created by Inchan Hwang on 2019-11-09.
//

#ifndef CHORD_CHORD_IMPL_H
#define CHORD_CHORD_IMPL_H

#include <utility>
#include <grpcpp/grpcpp.h>
#include "generated/chord.grpc.pb.h"
#include "context.h"
#include "chord_client.h"

using grpc::ServerContext;
using grpc::Status;
using chord::Chord;

using chord::JoinReq;
using chord::JoinResp;

using chord::GetInfoReq;
using chord::GetInfoResp;

using chord::FindSuccReq;
using chord::FindSuccResp;
using chord::FindPredReq;
using chord::FindPredResp;
using chord::ClosestPredFingerReq;
using chord::ClosestPredFingerResp;

using chord::StabilizeReq;
using chord::StabilizeResp;
using chord::NotifyReq;
using chord::NotifyResp;
using chord::FixFingersReq;
using chord::FixFingersResp;

class ChordImpl final : public Chord::Service {
private:
    Context myCtx;
    ChordClient client;

    void buildSuccessorList();

    // methods that call the right (local or remote) method for a given
    bool callGetInfo(Node target, chord::NodeInfo* dst);
    void callGetClosestFinger(Node target, uint32_t key, Node* dst);
    bool callFindPred(Node target, uint32_t key, Node* dst);
    bool callFindSucc(Node target, uint32_t key, Node* dst);
    void callNotify(Node target, const Node& potentialPred);

    // local handlers
    bool handleJoin(const Node& buddy);
    bool handleGetInfo(chord::NodeInfo* dst);
    bool handleFindSucc(uint32_t key, Node* dst);
    bool handleFindPred(uint32_t key, Node* dst);
    void handleGetClosestFinger(uint32_t key, Node* dst);
    void handleStabilize();
    void handleNotify(Node potentialPred);
    void handleFixFingers();


public:
    explicit ChordImpl(Context myContext);

    Status join(ServerContext* context, const JoinReq* request, JoinResp* response) override;
    Status getInfo(ServerContext* context, const GetInfoReq* request, GetInfoResp* response) override;
    Status findSucc(ServerContext* context, const FindSuccReq* request, FindSuccResp* response) override;
    Status findPred(ServerContext* context, const FindPredReq* request, FindPredResp* response) override;
    Status closestPredFinger(ServerContext* context, const ClosestPredFingerReq* request, ClosestPredFingerResp* response) override;
    Status stabilize(ServerContext* context, const StabilizeReq* request, StabilizeResp* response) override;
    Status notify(ServerContext* context, const NotifyReq* request, NotifyResp* response) override;
    Status fixFingers(ServerContext* context, const FixFingersReq* request, FixFingersResp* response) override;
};

#endif //CHORD_CHORD_IMPL_H
