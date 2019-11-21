#include <utility>

#include <utility>

//
// Created by Inchan Hwang on 2019-11-07.
//

#include <string>
#include "context.h"

using namespace std;

Context::Context(Node* me): me(me) {
    this->succ = nullptr;
    this->pred = nullptr;
}

void Context::setSucc(Node* succ_) {
    this->succ = succ_;
}

void Context::setPred(Node* pred_) {
    this->pred = pred_;
}

void Context::setFinger(int idx, Node* node) {
    tbl.setNode(idx, node);
}

Node* Context::getFinger(int idx) {
    return tbl.getNode(idx);
}

chord::NodeInfo* Context::genProto() {
    auto* proto = new chord::NodeInfo();

    chord::Node* meProto = me->genProto();
    chord::Node* succProto = succ->genProto();
    chord::Node* predProto = pred->genProto();

    proto->set_allocated_self(meProto);
    proto->set_allocated_succ(succProto);
    proto->set_allocated_pred(predProto);

    return proto;
}
