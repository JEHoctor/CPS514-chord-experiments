//
// Created by Inchan Hwang on 2019-11-07.
//

#include <utility>
#include <random>
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
        return !(to < toTest && toTest < from);
    }
}

bool ChordImpl::handleJoin(const Node& buddy) {
    Node succ;
    if(callFindSucc(buddy, myCtx.getMe().getID(), &succ)) {
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

    int cnt = 0;
    while(!info->succ().is_valid() || !isInside(n.getID()+1, info->succ().id(), key)) {
        callGetClosestFinger(n, key, &n);
        callGetInfo(n, info);
//        info->PrintDebugString();
        cnt ++;
    }
    printf("Handle findPred() completed in %d steps\n", cnt);

    dst->set(n.getAddr());
    return true;
}

bool ChordImpl::handleFindSucc(uint32_t key, Node* dst) {
    Node node;
    chord::NodeInfo info;
    if(handleFindPred(key, &node) && callGetInfo(node, &info) && info.succ().is_valid()) {
        dst->set(info.succ().addr());
        return true;
    }
    return false;
}

void ChordImpl::buildSuccessorList() {
    int predID = myCtx.getMe().getID();
    Node succ;
    chord::NodeInfo info;
    int cnt = 0;
    while(cnt < 32 && handleFindSucc(predID+1, &succ) && callGetInfo(succ, &info)) {
        myCtx.insertSucc(succ, predID);
        predID = succ.getID();
        cnt ++;
    }

//    printf("build successor list completed with %d %d\n", cnt, myCtx.size());
}

void ChordImpl::handleStabilize() {
    chord::NodeInfo info;

    if(myCtx.getSucc().getIsValid()) {
        if(callGetInfo(myCtx.getSucc(), &info)) { // yo you ok?
            auto pred = Node(info.pred());
            if(pred.getIsValid() && isInside(myCtx.getMe().getID()+1, myCtx.getSucc().getID()-1, pred.getID())) {
                myCtx.setSucc(pred);
            }
            buildSuccessorList();
            callNotify(myCtx.getSucc(), myCtx.getMe());
        } else {
//            printf("%s Something's wrong with my successor %s\n", myCtx.getMe().getAddr().c_str(), myCtx.getSucc().getAddr().c_str());
            myCtx.removeSucc();
            callNotify(myCtx.getSucc(), myCtx.getMe());
//            printf("It's Now %s\n", myCtx.getSucc().getAddr().c_str());
        }
    }
}

void ChordImpl::handleNotify(Node potentialPred) {
    Node pred = myCtx.getPred();
    chord::NodeInfo info;
    if(!callGetInfo(pred, &info) || isInside(pred.getID()+1, myCtx.getMe().getID()-1, potentialPred.getID())) {
        myCtx.setPred(potentialPred);
    }
}

void ChordImpl::handleFixFingers() {
//    int i = (std::rand() % 31) + 2; // [2, 32]
//
//    uint64_t id = myCtx.getMe().getID();
//    id += (1ull << (i-1));
//    id = id % (1ull << 32);
//
//    Node res;
//    if(handleFindSucc(id, &res)) {
//        myCtx.setFinger(i, res);
//    }
    for(int i = 1 ; i <= 32 ; i ++) {
        uint64_t id = myCtx.getMe().getID();
        id += (1ull << (i-1));
        id = id % (1ull << 32);

        Node res;
        chord::NodeInfo info;
        if(handleFindSucc(id, &res)) {
            if(i == 1) myCtx.setSucc(res);
            else myCtx.setFinger(i, res);
        }
    }
}


