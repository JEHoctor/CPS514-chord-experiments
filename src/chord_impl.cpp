#include <utility>

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

ChordImpl::ChordImpl(Context myContext): myCtx(std::move(myContext)) { }

Status ChordImpl::join(ServerContext *context, const JoinReq *request, JoinResp *response) {
    return handleJoin(Node(request->buddy())) ? Status::OK : Status::CANCELLED;
}

Status ChordImpl::getInfo(ServerContext *context, const GetInfoReq *request, GetInfoResp *response) {
    response->set_allocated_info(myCtx.genProto());
    return Status::OK;
}

Status ChordImpl::findPred(ServerContext *context, const FindPredReq *request, FindPredResp *response) {
    uint32_t key = request->key();

    Node node;
    if(!handleFindPred(key, &node)) return Status::CANCELLED;
    response->set_allocated_pred(node.genProto());

    return Status::OK;
}

Status ChordImpl::findSucc(ServerContext *context, const FindSuccReq *request, FindSuccResp *response) {
    uint32_t key = request->key();

    Node node;
    if(!handleFindSucc(key, &node)) return Status::CANCELLED;
    response->set_allocated_succ(node.genProto());

    return Status::OK;
}

Status ChordImpl::closestPredFinger(ServerContext *context, const ClosestPredFingerReq *request,
                                    ClosestPredFingerResp *response) {
    uint32_t key = request->key();

    Node node;
    handleGetClosestFinger(key, &node);
    response->set_allocated_pred_finger(node.genProto());

    return Status::OK;
}

Status ChordImpl::stabilize(ServerContext *context, const StabilizeReq *request, StabilizeResp *response) {
    handleStabilize();
    return Status::OK;
}

Status ChordImpl::notify(ServerContext *context, const NotifyReq *request, NotifyResp *response) {
    handleNotify(Node(request->potential_pred()));
    return Status::OK;
}

Status ChordImpl::fixFingers(ServerContext *context, const FixFingersReq *request, FixFingersResp *response) {
    handleFixFingers();
    return Status::OK;
}

bool ChordImpl::callGetInfo(Node target, chord::NodeInfo* dst) {
    if(target.getID() == myCtx.getMe().getID()) return handleGetInfo(dst);
    else return client.getInfo(target, dst);
}

void ChordImpl::callGetClosestFinger(Node target, uint32_t key, Node* dst) {
    if(target.getID() == myCtx.getMe().getID()) return handleGetClosestFinger(key, dst);
    else return client.getClosestFinger(target, key, dst);
}

bool ChordImpl::callFindPred(Node target, uint32_t key, Node* dst) {
    if(target.getID() == myCtx.getMe().getID()) return handleFindPred(key, dst);
    else return client.findPred(target, key, dst);
}

bool ChordImpl::callFindSucc(Node target, uint32_t key, Node* dst) {
    if(target.getID() == myCtx.getMe().getID()) return handleFindSucc(key, dst);
    else return client.findSucc(target, key, dst);
}

void ChordImpl::callNotify(Node target, const Node& potentialPred) {
    if(target.getID() == myCtx.getMe().getID()) return handleNotify(potentialPred);
    else return client.notify(target, potentialPred);
}

// handle handlers

bool isInside(uint32_t from, uint32_t to, uint32_t toTest) {
    if(from <= to) {
        return from <= toTest && toTest <= to;
    } else {
        return !(to <= toTest && toTest <= from);
    }
}

bool ChordImpl::handleJoin(Node buddy) {
    Node succ;
    if(callFindSucc(std::move(buddy), myCtx.getMe().getID(), &succ)) {
        myCtx.setSucc(succ);
        return true;
    }

    return false;
}

bool ChordImpl::handleGetInfo(chord::NodeInfo* dst) {
    chord::NodeInfo* info = myCtx.genProto();
    dst->CopyFrom(*info);
    delete info;
    return true;
}

void ChordImpl::handleGetClosestFinger(uint32_t key, Node* dst) {
    Node entry;
    for(int i = 32 ; i > 0 ; i --) {
        if(!myCtx.getFinger(i, &entry)) continue;
        if(isInside(myCtx.getMe().getID()+1, key-1, entry.getID())) {
            dst->set(entry.getAddr());
            return;
        }
    }
    dst->set(myCtx.getMe().getAddr());
}

bool ChordImpl::handleFindPred(uint32_t key, Node* dst) {
    Node n = myCtx.getMe();
    chord::NodeInfo* info = myCtx.genProto();

    while(!isInside(n.getID()+1, info->succ().id(), key)) {
        callGetClosestFinger(n, key, &n);
        delete info;
        callGetInfo(n, info);
    }
    delete info;

    dst->set(n.getAddr());
    return true;
}

bool ChordImpl::handleFindSucc(uint32_t key, Node* dst) {
    Node node;
    chord::NodeInfo info;
    if(callFindPred(myCtx.getMe(), key, &node) && callGetInfo(node, &info)) {
        dst->set(info.succ().addr());
        return true;
    }
    return false;
}

void ChordImpl::handleStabilize() {
    chord::NodeInfo info;
    if(callGetInfo(myCtx.getSucc(), &info)) {
        auto pred = Node(info.pred());
        if(isInside(myCtx.getMe().getID()+1, myCtx.getSucc().getID()-1, pred.getID()) {
            myCtx.setSucc(pred);
        }
        callNotify(myCtx.getSucc(), myCtx.getMe());
    }
}

void ChordImpl::handleNotify(Node potentialPred) {
    Node pred = myCtx.getPred();
    if(!pred.getIsValid() || isInside(pred.getID()+1, myCtx.getMe().getID()-1, potentialPred.getID())) {
        pred.set(potentialPred.getAddr());
    }
}

void ChordImpl::handleFixFingers() {
    
}


