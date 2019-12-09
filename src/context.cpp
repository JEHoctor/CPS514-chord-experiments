//
// Created by Inchan Hwang on 2019-11-07.
//

#include <string>
#include "context.h"

using namespace std;

Context::Context(Node me): me(me), succs(SuccessorList(me.getID())) {
    setSucc(me);
    pred = Node();
}

Node Context::getMe() { return me; }
Node Context::getSucc() { return succs.successor(); }
Node Context::getPred() { return pred; }

void Context::setSucc(const Node& succ_) {
    succs.addNode(succ_, me.getID());
    setFinger(1, succ_);
}

void Context::setPred(Node pred_) {
    this->pred = std::move(pred_);
}

void Context::setFinger(int idx, Node node) {
    if(node.getIsValid()) {
        tbl.setNode(idx, std::move(node));
    } else {
        tbl.delNode(idx);
    }
}

bool Context::getFinger(int idx, Node* dst) {
    return tbl.getNode(idx, dst);
}

void Context::insertSucc(Node succ_, int predID) {
    succs.addNode(std::move(succ_), predID);
}

void Context::removeSucc() {
    succs.removeNode(succs.successor().getID());
}

void Context::removeSucc(int id) {
    succs.removeNode(id);
}

chord::NodeInfo* Context::genProto() {
    auto* proto = new chord::NodeInfo();

    chord::Node* meProto = me.genProto();
    chord::Node* succProto = succs.successor().genProto();
    chord::Node* predProto = pred.genProto();

    proto->set_allocated_self(meProto);
    proto->set_allocated_succ(succProto);
    proto->set_allocated_pred(predProto);

    return proto;
}
