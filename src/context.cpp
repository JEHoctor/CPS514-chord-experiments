#include <utility>

#include <utility>

#include <utility>

#include <utility>

//
// Created by Inchan Hwang on 2019-11-07.
//

#include <string>
#include "context.h"

using namespace std;

Context::Context(const Node& me): me(me) {
    this->succ = me;
    this->pred = Node();
}

Node Context::getMe() { return me; }
Node Context::getSucc() { return succ; }
Node Context::getPred() { return pred; }

void Context::setSucc(Node succ_) {
    this->succ = std::move(succ_);
    setFinger(1, this->succ);
}

void Context::setPred(Node pred_) {
    this->pred = std::move(pred_);
}

void Context::setFinger(int idx, Node node) {
    tbl.setNode(idx, std::move(node));
}

bool Context::getFinger(int idx, Node* dst) {
    return tbl.getNode(idx, dst);
}

chord::NodeInfo* Context::genProto() {
    auto* proto = new chord::NodeInfo();

    chord::Node* meProto = me.genProto();
    chord::Node* succProto = succ.genProto();
    chord::Node* predProto = pred.genProto();

    proto->set_allocated_self(meProto);
    proto->set_allocated_succ(succProto);
    proto->set_allocated_pred(predProto);

    return proto;
}
