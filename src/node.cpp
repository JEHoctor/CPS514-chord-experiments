//
// Created by Inchan Hwang on 2019-11-21.
//

#include "node.h"

Node::Node(const std::string &addr): addr(addr) {
    id = extractID(addr);
}

chord::Node* Node::genProto() {
    auto* proto = new chord::Node();
    proto->set_addr(this->addr);
    proto->set_id(this->id);
    return proto;
}